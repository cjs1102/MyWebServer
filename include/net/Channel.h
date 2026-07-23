#pragma once

#include <functional>

using namespace std;

// Channel
//
// 一个fd的事件封装
//
// 负责保存：
//
// 1. socket fd
// 2. 监听事件
// 3. 事件发生后的回调函数

class Channel
{
public:
    Channel(int fd);

    int fd() const;

    // 设置读事件回调函数
    void setReadCallback(function<void()> cb);

    void setWriteCallback(
        function<void()> cb);

    // 处理事件
    //
    //  当epoll发现fd可读
    //  调用这个函数
    void handleEvent();

    void handleWrite();

    void disable();

    bool enabled() const;

private:
    int fd_;

    bool enabled_;

    // 读事件回调函数
    function<void()> readCallback_;

    function<void()> writeCallback_;
};