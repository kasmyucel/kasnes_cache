#include "threadpool.h"

ThreadPool::ThreadPool(size_t num_threads)
{

    for (size_t i = 0; i < num_threads; ++i) {
        threads_.emplace_back([this] {
            while (true) {
                function<void()> task;
                {
                    unique_lock<mutex> lock(queue_mutex_);

                    cv_.wait(lock, [this] {
                        return !tasks_.empty() || stop_;
                    });

                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        unique_lock<mutex> lock(queue_mutex_);
        stop_ = true;
    }

    // Notify all threads
    cv_.notify_all();

    for (auto& thread : threads_) {
        thread.join();
    }
}

// Enqueue task for execution by the thread pool
void ThreadPool::enqueue(function<void()> task)
{
    {
        unique_lock<std::mutex> lock(queue_mutex_);
        tasks_.emplace(std::move(task));
    }
    cv_.notify_one();
}
