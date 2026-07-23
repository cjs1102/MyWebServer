#include "thread/EventLoopThread.h"

EventLoopThread::EventLoopThread()
    : loop_(nullptr)
{
}

EventLoopThread::~EventLoopThread()
{

    if (thread_.joinable())
    {
        thread_.join();
    }
}

EventLoop *EventLoopThread::startLoop()
{

    thread_ =
        thread(
            &EventLoopThread::threadFunc,
            this);

    unique_lock<mutex> lock(mutex_);

    cond_.wait(
        lock,
        [this]()
        {
            return loop_ != nullptr;
        });

    return loop_;
}

void EventLoopThread::threadFunc()
{

    EventLoop loop;

    {
        lock_guard<mutex> lock(mutex_);

        loop_ = &loop;

        cond_.notify_one();
    }

    loop.loop();
}