#pragma once

#include "net/Epoll.h"
#include "net/Channel.h"
#include "timer/TimerManager.h"

#include <thread>
#include <functional>
#include <vector>
#include <mutex>

#include <sys/eventfd.h>
#include <unordered_map>
// EventLoop
// 事件循环类
//
// 作用：
// 1. 持续运行服务器主循环
// 2. 调用epoll等待事件
// 3. 分发处理网络事件

class EventLoop
{
public:
    EventLoop();

    // 启动事件循环
    void loop();

    // 添加Channel
    //
    void addChannel(
        Channel *channel);
    // 删除Channel
    //
    void removeChannel(
        int fd);

    // 跨线程执行任务
    void runInLoop(
        function<void()> cb);

    // 修改监听事件
    void modifyChannel(
        int fd,
        uint32_t events);

    // 添加定时器
    void addTimer(
        shared_ptr<Timer> timer);

    bool isInLoopThread();

    void assertInLoopThread();

    void queueInLoop(
        function<void()> cb);

private:
    void wakeup();

    void handleWakeup();

    // 创建EventLoop的线程id
    thread::id threadId_;

private:
    // epoll对象
    //
    // 负责监听多个socket事件
    Epoll epoll_;

    // fd到Channel映射
    //
    // key:
    // socket fd
    //
    // value:
    // 对应Channel
    unordered_map<int, Channel *> channels_;

    // 定时器管理
    TimerManager timerManager_;

    // 唤醒fd

    int wakeupFd_;

    Channel *wakeupChannel_;

    // 任务队列

    vector<function<void()>> pendingTasks_;

    mutex mutex_;
};