#pragma once

#include "net/EventLoop.h"
#include "thread/EventLoopThread.h"

#include <vector>

using namespace std;

class EventLoopThreadPool
{

public:
    EventLoopThreadPool(int numThreads);

    ~EventLoopThreadPool();

    // 创建IO线程
    void start();

    // 获取一个EventLoop
    EventLoop *nextLoop();

private:
    // IO线程数量
    int numThreads_;

    // EventLoopThread对象
    vector<EventLoopThread *> threads_;

    // 对应EventLoop
    vector<EventLoop *> loops_;

    // 轮询下标
    int next_;
};