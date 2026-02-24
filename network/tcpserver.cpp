#include "tcpserver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

TCPServer::TCPServer(int port_, ThreadPool& threadPool_,ConnectionPool& connectionPool_, DataService& dataService_):
    port(port_),
    isRunning(false),
    serverFd(-1),
    epollFd(-1),
    shutdownFd(-1),
    threadPool(threadPool_),
    connectionPool(connectionPool_),
    dataService(dataService_)
{
    if(initialize() != 0){
        perror("Creating the server failed try again.");
    }
}

TCPServer::~TCPServer() {
    stop();
    if (serverFd != -1) {
        close(serverFd);
    }
    if (epollFd != -1) {
        close(epollFd);
    }
    if (shutdownFd != -1){
        close(shutdownFd);
    }
}

/*
 * Initialize the server before starting to use it.
 * SO_REUSEADDR set to allow using the same port right after a process.
 */
int TCPServer::initialize(){
    //TCP server settings.
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if(serverFd <0)
        return -1;

    //Set the SO_REUSEADDR option.
    int opt = 1;
    if(setsockopt(serverFd, SOL_SOCKET,SO_REUSEADDR,&opt , sizeof(opt))<0)
        return -1 ;


    //Server settings.
    sockaddr_in serverAddress ;
    serverAddress.sin_family = AF_INET ;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Bind the server
    if(bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress))<0){
        return - 1;
    }

    //Setting Epoll
    epollFd = epoll_create1(0);
    if (epollFd < 0){
        return -1;
    }

    shutdownFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (shutdownFd < 0){
        return -1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = serverFd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &ev) < 0) {
        return -1;
    }

    struct epoll_event evStop;
    evStop.events = EPOLLIN;
    evStop.data.fd = shutdownFd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, shutdownFd, &evStop) < 0) {
        return -1;
    }
    return 0;
}

/*
 * Start the server to listen and accept clients
 */
int TCPServer::start(){
    if(isRunning){
        perror("The server is already running.");
        return -1;
    }
    else{
        if(listen(serverFd, 20) != 0)
            return -1;

        isRunning = true;

        const int MAX_EVENTS = 10;
        struct epoll_event events[MAX_EVENTS];

        while(isRunning) {
            int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);

            if (nfds < 0) {
                perror("Epoll wait error");
                break;
            }

            for(int i = 0; i < nfds; i++) {

                if(events[i].data.fd == serverFd){
                sockaddr_in clientAddr;
                socklen_t clientLen = sizeof(clientAddr);
                int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);

                if (clientFd >= 0) {

                    ClientConnection* clientConnection = connectionPool.acquire();

                    if(clientConnection){
                        clientConnection->reset(clientFd, clientAddr);

                    threadPool.enqueue([this, clientConnection] {
                        this->processClient(clientConnection);
                    });
                    }
                    else{
                        perror("Connection cannot be acquired.");
                        close(clientFd);
                    }
                }
                else{
                    perror("Server accept error.");
                }
                }
                else if (events[i].data.fd == shutdownFd) {
                    isRunning = false;
            }
            }
            }
        }
        return 0;
    }


/*
 * Stop the server.
 */
int TCPServer::stop(){
    if(!isRunning){
        perror("The server is not running.");
        return -1;
    }
    else{
        uint64_t u = 1;
        ssize_t s = write(shutdownFd, &u, sizeof(uint64_t));

        if (s != sizeof(uint64_t)) {
            perror("Shutdown signal write failed");
        }
        return 0;
    }
}

/*
 * Sending data
 */
int TCPServer::sendData(int clientFd, Data& data){
    int size = sizeof(data);
    int bytesSend = send(clientFd,(char*) &data,size,0);
    if(bytesSend < size){
        perror("The data is not send correctly.");
        return -1;
    }
    else{
        return 0;
    }
}

/*
 * Receiving data for PUT operation
 */
Data TCPServer::receiveData(int clientFd){

    Data data ;
    int size = sizeof(Data);
    int bytesRead = recv(clientFd, (char*) &data, size, 0);
    if (bytesRead < size) {
        perror("The data is not received correctly.");
        return data;
    }
    else{
        return data;
    }

}

/*
 * Function that receives the id that program will search for GET operation.
 */
int TCPServer::receiveId(int clientFd){
    int result = -1 ;
    int bytesRead = recv(clientFd, (char*) &result, sizeof(int), 0);
    if (bytesRead < sizeof(int)) {
        perror("The data is not received correctly.");
        return -1 ;
    }
    else{
        return result;
    }
}

/*
 * Handle the process with protocol
 * It will be send in ThreadPool and will be called there.
 */
int TCPServer::processClient(ClientConnection* clientConnection){

    int requestedId = receiveId(clientConnection->fd);

    if (requestedId != -1) {

        Data response;
        bool found = dataService.getData(requestedId, response);

        if (found) {
            sendData(clientConnection->fd, response);
            connectionPool.release(clientConnection);
            return 0;
        }
        else {
            // Sends a data that indicates that data is not found. This line is optional we can send John Doe Wallet
            Data notFound("Not Found", 0.0, requestedId);
            sendData(clientConnection->fd, notFound);

            perror("Data is not found on the server.");
            connectionPool.release(clientConnection);
            return -1;
        }

    } else {
        connectionPool.release(clientConnection);
        return -1 ;
    }
}
