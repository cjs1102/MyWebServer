#pragma once

#include <string>
#include <errno.h>

#include "net/Channel.h"
#include "net/EventLoop.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "timer/Timer.h"

#include <memory>
#include <functional>
using namespace std;

class TcpServer;
// TcpConnection
// 一个客户端连接对象
// 负责
// 1. 管理客户端socket
// 2. 创建Channel
// 3. 响应读事件
// 4. 处理HTTP请求
//
enum State
{
    Connecting,
    Connected,
    Disconnecting,
    Disconnected
};

class TcpConnection :

    public enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(
        int fd,
        EventLoop *loop,
        TcpServer *server);

    ~TcpConnection();

    // 初始化连接
    //
    // 注册事件
    void establish();

    // 处理读事件
    void handleRead();

    // 发送数据给客户端
    void send(const string &data);

    // http业务处理
    void handleRequest(
        const string &request);

    void handleWrite();

    // 刷新连接超时时间
    void resetTimer();

    // 关闭连接
    void close();

    // 设置关闭回调
    void setCloseCallback(
        function<void(int)> cb);

    bool connected() const;

private:
    void enableWrite();

    void disableWrite();
    bool reading_;

    // 优雅关闭
    void shutdown();

    State state_;

    // 关闭通知
    function<void(int)> closeCallback_;

private:
    void closeInLoop();

    // 客户端socket文件描述符
    int fd_;

    // 对应Channel
    //
    Channel *channel_;

    EventLoop *loop_;

    // 所属服务器
    TcpServer *server_;

    // http接收缓冲区
    string readBuffer_;

    // 输出缓冲区
    //
    // 保存暂时无法发送的数据
    string writeBuffer_;

    // 空闲超时Timer
    shared_ptr<Timer> timer_;

    // 是否关闭
    bool closed_;
};