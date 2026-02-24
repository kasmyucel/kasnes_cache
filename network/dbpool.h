#ifndef DBPOOL_H
#define DBPOOL_H

#include "network/dbconnection.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class DBPool
{   
public:
    DBPool(int size);

private:
    std::queue<DBConnection*> connections;
    std::mutex mtx;
    std::condition_variable cv;
    int poolSize;

public:

    // A helper wrapper for RAII principles to
    // automate connection acquire-release workflow.
    class ConnectionGuard {
        DBPool& pool;
        DBConnection* conn;
    public:
        ConnectionGuard(DBPool& p) : pool(p) {
            conn = pool.acquire();
        }
        ~ConnectionGuard() {
            pool.release(conn);
        }
        DBConnection* operator->() { return conn; }
    };

    DBConnection* acquire() {
        std::unique_lock<std::mutex> lock(mtx);

        // If there is no any available connection in the pool, wait
        // since there are limited connections for datasbases.
        cv.wait(lock, [this] { return !connections.empty(); });

        DBConnection* conn = connections.front();
        connections.pop();
        return conn;
    }

    void release(DBConnection* conn) {
        std::lock_guard<std::mutex> lock(mtx);
        connections.push(conn);
        cv.notify_one(); // Notify if there is a waiting thread
    }

};

#endif // DBPOOL_H
