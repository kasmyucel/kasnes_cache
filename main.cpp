#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <thread>
#include <chrono>
#include <atomic>
#include "base/lrucache.h"
#include "base/data.h"
#include "base/threadpool.h"
#include "network/connectionpool.h"
#include "network/dbpool.h"
#include "base/dataservice.h"
#include "network/tcpserver.h"

using namespace std;

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"


void runServer(TCPServer* server)
{
    server->start();
}

int main()
{
    DBPool dbPool(5);
    LRUCache cache(100);
    ThreadPool threadPool(4);
    ConnectionPool connPool(20);
    DataService service(cache, dbPool);
    TCPServer server(8080, threadPool, connPool, service);
    std::thread serverThread(runServer, &server);

    return 0;
}
