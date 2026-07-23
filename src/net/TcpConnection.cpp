#include "net/TcpConnection.h"

#include "log/Logger.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "net/TcpServer.h"
#include "timer/Timer.h"

#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <cstring>

TcpConnection::TcpConnection(
    int fd, EventLoop *loop, TcpServer *server)
    : fd_(fd), channel_(nullptr),
      loop_(loop), server_(server), closed_(false),
      state_(Connecting), reading_(true)
{
}

TcpConnection::~TcpConnection()
{
    if (timer_)
    {
        timer_->cancel();
    }
    if (channel_)
    {
        delete channel_;
        channel_ = nullptr;
    }

    if (fd_ != -1)
    {
        ::close(fd_);
        fd_ = -1;
    }
}

bool TcpConnection::connected() const
{
    return state_ == Connected;
}

void TcpConnection::shutdown()
{

    if (state_ == Connected)
    {

        state_ = Disconnecting;

        if (writeBuffer_.empty())
        {
            close();
        }
    }
}

void TcpConnection::establish()
{
    // 创建Channel
    channel_ = new Channel(fd_);

    // 设置读事件回调
    channel_->setReadCallback(
        [this]()
        {
            handleRead();
        });

    channel_->setWriteCallback(
        [this]()
        {
            handleWrite();
        });
    // 注册到epoll
    loop_->addChannel(channel_);

    state_ = Connected;

    // 开始连接计时
    resetTimer();
}

void TcpConnection::handleRead()
{
    char buffer[4096] = {0};

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int len = recv(fd_,
                       buffer,
                       sizeof(buffer),
                       0);
        if (len > 0)
        {
            readBuffer_.append(buffer, len);
            // 刷新连接超时时间
            resetTimer();

            Logger::debug(
                "client data:\n" + readBuffer_);
        }
        else if (len == 0)
        {
            // 客户端关闭
            Logger::info(
                "client closed");

            close();

            return;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // 数据读取完毕
                break;
            }
            Logger::error(
                "recv error");
            close();

            return;
        }

        Logger::debug("client data:\n" + readBuffer_);

        // 判断HTTP请求是否完整
        size_t pos =
            readBuffer_.find(
                "\r\n\r\n");

        if (pos == string::npos)
        {
            // header还没有接收完整

            break;
        }

        string header =
            readBuffer_.substr(
                0,
                pos + 4);

        size_t bodyLength = 0;

        size_t lengthPos =
            header.find(
                "Content-Length:");

        if (lengthPos != string::npos)
        {

            size_t start =
                lengthPos +
                strlen("Content-Length:");

            while (
                header[start] == ' ')
            {
                start++;
            }

            bodyLength =
                stoi(
                    header.substr(start));
        }

        size_t totalLength =
            pos + 4 + bodyLength;

        // 请求还没收完整
        if (readBuffer_.size() < totalLength)
        {
            return;
        }

        // 一个完整HTTP请求
        string request =
            readBuffer_.substr(
                0,
                totalLength);

        readBuffer_.erase(
            0,
            totalLength);

        //
        // 后面这里处理HTTP
        //

        Logger::debug(
            "complete http request:\n" +
            request);

        handleRequest(request);
    }
}
void TcpConnection::handleRequest(
    const string &request)
{
    HttpRequest req;

    HttpResponse response;

    if (!req.parse(request))
    {

        response.setStatusCode(400);

        response.setBody(
            "<h1>400 Bad Request</h1>");
        send(
            response.makeResponse());

        return;
    }

    string method =
        req.method();

    if (method == "GET")
    {
        string path =
            req.path();

        if (path == "/")
        {
            path = "/index.html";
        }

        if (!response.readFile(
                "www" + path))
        {
            response.setStatusCode(404);
            response.setBody(
                "<h1>404 Not Found</h1>");
        }
    }
    else if (method == "POST")
    {

        Logger::debug(
            "POST body:\n" +
            req.body());

        response.setBody(
            "<h1>POST Success</h1>");
    }
    else
    {

        response.setStatusCode(405);
        response.setBody(
            "<h1>405 Method Not Allowed</h1>");
    }

    bool keepAlive = true;

    if (req.header("Connection") == "close")
    {
        keepAlive = false;
    }

    if (keepAlive)
    {

        response.setHeader(
            "Connection",
            "keep-alive");
    }
    else
    {

        response.setHeader(
            "Connection",
            "close");

        state_ = Disconnecting;
    }

    send(
        response.makeResponse());
}

void TcpConnection::send(const string &data)
{
    auto self =
        shared_from_this();

    loop_->runInLoop(
        [self, data]()
        {
            if (self->fd_ == -1)
            {
                return;
            }
            self->writeBuffer_ += data;

            self->enableWrite();
        });
}

void TcpConnection::enableWrite()
{

    loop_->modifyChannel(
        fd_,
        EPOLLIN |
            EPOLLOUT |
            EPOLLET);
}

void TcpConnection::disableWrite()
{

    loop_->modifyChannel(
        fd_,
        EPOLLIN |
            EPOLLET);
}

void TcpConnection::handleWrite()
{

    if (writeBuffer_.empty())
    {
        disableWrite();

        if (state_ == Disconnecting)
        {
            close();
        }

        return;
    }
    while (true)
    {

        int len =
            ::send(
                fd_,
                writeBuffer_.data(),
                writeBuffer_.size(),
                0);

        if (len > 0)
        {
            writeBuffer_.erase(
                0,
                len);

            if (writeBuffer_.empty())
            {
                disableWrite();
                // 关闭EPOLLOUT
                break;
            }
        }
        else
        {
            if (errno == EAGAIN ||
                errno == EWOULDBLOCK)
            {
                // 发送缓冲区满
                // 等待下一次EPOLLOUT

                break;
            }

            Logger::error(
                "send error");

            close();
            break;
        }
    }
}

void TcpConnection::resetTimer()
{
    // 取消旧Timer
    if (timer_)
    {
        timer_->cancel();
    }

    auto self = shared_from_this();

    timer_ =
        make_shared<Timer>(
            60,
            [self]()
            {
                Logger::info(
                    "connection timeout");

                self->close();
            });

    loop_->addTimer(timer_);
}

void TcpConnection::setCloseCallback(
    function<void(int)> cb)
{

    closeCallback_ = cb;
}

void TcpConnection::close()
{

    if (state_ == Disconnected)
    {
        return;
    }

    // 如果当前就是IO线程
    if (loop_->isInLoopThread())
    {
        closeInLoop();
    }
    else
    {
        // 投递到IO线程执行
        auto self = shared_from_this();

        loop_->runInLoop(
            [this]()
            {
                closeInLoop();
            });
    }
}

void TcpConnection::closeInLoop()
{

    if (state_ == Disconnected)
    {
        return;
    }

    state_ = Disconnected;

    Logger::info(
        "close connection");

    int oldfd = fd_;

    if (timer_)
    {
        timer_->cancel();
    }

    // 这里一定在IO线程执行
    loop_->removeChannel(oldfd);

    ::close(oldfd);

    fd_ = -1;

    if (closeCallback_)
    {
        closeCallback_(oldfd);
    }
}