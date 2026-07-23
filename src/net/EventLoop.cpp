#include "net/EventLoop.h"
#include "net/Channel.h"
#include <unistd.h>
#include "net/Epoll.h"
#include "log/Logger.h"

// 构造函数
EventLoop::EventLoop()
    : threadId_(this_thread::get_id())
{
    Logger::info("EventLoop created");

    wakeupFd_ =
        eventfd(
            0,
            EFD_NONBLOCK);

    wakeupChannel_ =
        new Channel(
            wakeupFd_);

    wakeupChannel_->setReadCallback(
        [this]()
        {
            handleWakeup();
        });

    addChannel(
        wakeupChannel_);
}

void EventLoop::modifyChannel(
    int fd,
    uint32_t events)
{

    assertInLoopThread();

    epoll_.modifyFd(
        fd,
        events);
}

bool EventLoop::isInLoopThread()
{
    return threadId_ ==
           this_thread::get_id();
}

void EventLoop::assertInLoopThread()
{

    if (!isInLoopThread())
    {
        Logger::error(
            "EventLoop used in wrong thread");

        abort();
    }
}

void EventLoop::wakeup()
{

    uint64_t one = 1;

    write(
        wakeupFd_,
        &one,
        sizeof(one));
}

void EventLoop::handleWakeup()
{

    uint64_t one;

    read(
        wakeupFd_,
        &one,
        sizeof(one));

    vector<function<void()>> tasks;

    {
        lock_guard<mutex> lock(
            mutex_);

        tasks.swap(
            pendingTasks_);
    }

    for (auto &task : tasks)
    {
        task();
    }
}

void EventLoop::runInLoop(
    function<void()> cb)
{
    if (isInLoopThread())
    {
        cb();
        return;
    }

    {
        lock_guard<mutex> lock(
            mutex_);

        pendingTasks_.push_back(
            cb);
    }

    wakeup();
}

void EventLoop::addChannel(Channel *channel)
{
    assertInLoopThread();
    // 保存fd和Channel关系
    channels_[channel->fd()] = channel;
    // 注册到epoll
    epoll_.addFd(channel->fd());
}

void EventLoop::removeChannel(int fd)
{
    auto it = channels_.find(fd);

    if (it != channels_.end())
    {
        assertInLoopThread();

        it->second->disable();

        epoll_.removeFd(fd);

        channels_.erase(it);
    }
}

void EventLoop::queueInLoop(
    function<void()> cb)
{

    {
        lock_guard<mutex> lock(
            mutex_);

        pendingTasks_.push_back(cb);
    }

    wakeup();
}

// 启动事件循环
//
//  服务器核心循环
void EventLoop::loop()
{
    Logger::info("EventLoop start");

    while (true)
    {

        int timeout = timerManager_.getNextTimeout();
        auto events = epoll_.wait(timeout);

        // 处理事件,遍历所有发生事件的fd
        for (auto &event : events)
        {
            int fd = event.data.fd;

            // 处理网络事件
            Logger::info("Event occurred on fd:" +
                         to_string(fd));

            // 找到对应的Channel
            auto it = channels_.find(fd);

            if (it == channels_.end())
            {
                continue;
            }

            Channel *channel = it->second;

            // 读事件
            if (event.events & EPOLLIN)
            {
                channel->handleEvent();
            }

            // 写事件
            if (event.events & EPOLLOUT)
            {
                channel->handleWrite();
            }
        }

        // 检查定时器
        timerManager_
            .handleExpired();
    }
}

void EventLoop::addTimer(
    shared_ptr<Timer> timer)
{

    timerManager_.addTimer(timer);
}