#include "thread/ThreadPool.h"

ThreadPool::ThreadPool(int size)
{
    for (int i = 0; i < size; i++)
    {
        workers.emplace_back(
            [this]()
            {
                while (true)
                {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(mutex_);
                        condition_.wait(lock, [this]()
                                        { return !tasks.empty(); });
                        task = tasks.front();
                        tasks.pop();
                    }
                    task();
                }
            });
    }
}

void ThreadPool::submit(function<void()> task)
{
    {
        lock_guard<mutex> lock(mutex_);
        tasks.push(task);
    }

    condition_.notify_one();
}