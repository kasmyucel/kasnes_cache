#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

class ThreadPool {
public:
    ThreadPool(size_t num_threads = thread::hardware_concurrency());
    ~ThreadPool();
    void enqueue(function<void()> task);



private:
    vector<thread> threads_;

    queue<function<void()> > tasks_;

    mutex queue_mutex_;

    condition_variable cv_;

    bool stop_ = false;
};
#endif // THREADPOOL_H
