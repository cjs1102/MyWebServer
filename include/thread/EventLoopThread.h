#pragma once

#include "net/EventLoop.h"

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class EventLoopThread
{

public:
    EventLoopThread();

    ~EventLoopThread();

    // 启动线程
    EventLoop *startLoop();

private:
    void threadFunc();

private:
    thread thread_;

    EventLoop *loop_;

    mutex mutex_;

    condition_variable cond_;
};