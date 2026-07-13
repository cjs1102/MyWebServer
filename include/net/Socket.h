#ifndef SOCKET_H
#define SOCKET_H

class Socket
{
public:
    // 构造函数
    // 创建TCP socket
    Socket();

    // 析构函数
    // 关闭socket
    ~Socket();

    // 获取socket文件描述符
    int fd();

private:
    // Linux socket 句柄
    int sockfd;
};

#endif