#include "connectionpool.h"

ConnectionPool::ConnectionPool(int size) {
    for(int i = 0 ; i<size ; i++){
        ClientConnection* clientConnection = new ClientConnection();
        pool.push_back(clientConnection);
        available.push(clientConnection);
    }
}

ConnectionPool::~ConnectionPool(){
    for(ClientConnection* connection : pool){
        delete connection ;
    }
}

ClientConnection* ConnectionPool::acquire(){
    std::unique_lock<std::mutex> lock(mutex);

    // Wait if the pool is empty
    cv.wait(lock, [this] { return !available.empty(); });

    ClientConnection* clientConnection = available.front();
    available.pop();
    return clientConnection;
}

void ConnectionPool::release(ClientConnection* clientConnection) {
    clientConnection->closeConnection();

    std::lock_guard<std::mutex> lock(mutex);
    available.push(clientConnection);


    cv.notify_one();
}
