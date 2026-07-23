#include "net/TcpServer.h"

#include "net/TcpConnection.h"

#include "log/Logger.h"

#include <errno.h>

TcpServer::TcpServer(uint16_t port)

    : port_(port),
      acceptChannel_(nullptr),
      threadPool_(3)

{
}

TcpServer::~TcpServer()
{

    connections_.clear();

    if (acceptChannel_)
    {
        delete acceptChannel_;

        acceptChannel_ = nullptr;
    }
}

void TcpServer::start()
{

    Logger::info(
        "server start");

    /*
        启动IO线程
    */

    threadPool_.start();

    /*
        创建监听socket
    */

    socket_.bind(port_);

    socket_.setNonBlocking();

    socket_.listen(10);

    int listenfd =
        socket_.fd();

    acceptChannel_ =
        new Channel(listenfd);

    acceptChannel_->setReadCallback(
        [this]()
        {
            handleAccept();
        });

    loop_.addChannel(
        acceptChannel_);

    /*
        main reactor启动
    */

    loop_.loop();
}

void TcpServer::handleAccept()
{

    while (true)
    {

        int client =
            socket_.accept();

        if (client < 0)
        {

            if (errno == EAGAIN ||
                errno == EWOULDBLOCK)
            {
                break;
            }

            Logger::error(
                "accept error");

            break;
        }

        Logger::info(
            "new client");

        Socket::setNonBlocking(
            client);

        /*
            获取IO线程
        */

        EventLoop *ioLoop =
            threadPool_.nextLoop();

        auto conn =
            make_shared<TcpConnection>(
                client,
                ioLoop,
                this);

        conn->setCloseCallback(
            [this](int fd)
            {
                removeConnection(fd);
            });

        connections_[client] = conn;

        /*
            必须在IO线程创建Channel
        */

        ioLoop->runInLoop(
            [conn]()
            {
                conn->establish();
            });
    }
}

void TcpServer::removeConnection(
    int fd)
{

    loop_.runInLoop(
        [this, fd]()
        {
            auto it =
                connections_.find(fd);

            if (it != connections_.end())
            {

                connections_.erase(it);

                Logger::info(
                    "connection removed");
            }
        });
}