#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;

class ThreadPool
{

public:
    ThreadPool(int size);

    void submit(function<void()> task);

private:
    vector<thread> workers;

    queue<function<void()>> tasks;

    mutex mutex_;

    condition_variable condition_;
};