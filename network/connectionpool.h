#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

struct ClientConnection
{
    int fd = -1;
    sockaddr_in addr = {};

    void reset(int clientFd, sockaddr_in clientAddr){
        fd = clientFd;
        addr = clientAddr ;
    }
    void closeConnection(){
        if(fd != -1){
            close(fd);
            fd = -1;
        }
    }
};

class ConnectionPool
{
private:
    std::vector<ClientConnection*> pool ;
    std::queue<ClientConnection*> available ;
    std::mutex mutex;
    std::condition_variable cv;

public:
    ConnectionPool(int size);
    ~ConnectionPool();
    ClientConnection* acquire();
    void release(ClientConnection* clientConnection);
};

#endif

