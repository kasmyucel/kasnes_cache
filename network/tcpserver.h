#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "base/threadpool.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "base/data.h"
#include "base/threadpool.h"
#include "network/connectionpool.h"
#include "base/dataservice.h"

class TCPServer
{
private:
    int serverFd;
    int port ;
    bool isRunning;
    int epollFd;
    int shutdownFd;

    ThreadPool& threadPool;
    ConnectionPool& connectionPool;
    DataService& dataService;

public:
    TCPServer(int port_, ThreadPool& threadPool_,ConnectionPool& connectionPool_, DataService& dataService_);
    ~TCPServer();
    int start();
    int stop();
    int processClient(ClientConnection* clientConnection);
private:
    int initialize();
    int sendData(int clientFd, Data& data);
    Data receiveData(int clientFd);
    int receiveId(int clientFd);
};

#endif // TCPSERVER_H
