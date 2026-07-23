#include "thread/EventLoopThreadPool.h"

#include "log/Logger.h"

EventLoopThreadPool::
    EventLoopThreadPool(int numThreads)

    : numThreads_(numThreads),
      next_(0)

{
}

EventLoopThreadPool::
    ~EventLoopThreadPool()
{

    for (auto thread : threads_)
    {
        delete thread;
    }

    threads_.clear();
    loops_.clear();
}

void EventLoopThreadPool::start()
{

    for (int i = 0; i < numThreads_; i++)
    {

        EventLoopThread *thread =
            new EventLoopThread();

        threads_.push_back(thread);

        EventLoop *loop =
            thread->startLoop();

        loops_.push_back(loop);

        Logger::info(
            "IO EventLoop started");
    }
}

EventLoop *
EventLoopThreadPool::nextLoop()
{

    if (loops_.empty())
    {
        return nullptr;
    }

    EventLoop *loop =
        loops_[next_];

    next_++;

    if (next_ >= loops_.size())
    {
        next_ = 0;
    }

    return loop;
}