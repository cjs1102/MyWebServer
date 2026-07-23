#pragma once

#include "net/Socket.h"

#include "net/EventLoop.h"

#include "net/Channel.h"

#include "thread/EventLoopThreadPool.h"

#include <unordered_map>
#include <memory>

using namespace std;

class TcpConnection;

class TcpServer
{

public:
    TcpServer(uint16_t port);

    ~TcpServer();

    // 启动服务器
    void start();

private:
    // 新连接
    void handleAccept();

    // 删除连接
    void removeConnection(int fd);

private:
    // 监听端口
    uint16_t port_;

    // listen socket
    Socket socket_;

    // 主Reactor
    //
    // 负责accept
    EventLoop loop_;

    // listenfd Channel
    Channel *acceptChannel_;

    // IO线程池
    EventLoopThreadPool threadPool_;

    // 客户端连接
    unordered_map<
        int,
        shared_ptr<TcpConnection>>
        connections_;
};