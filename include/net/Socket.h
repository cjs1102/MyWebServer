#ifndef SOCKET_H
#define SOCKET_H // 防止重复包含 避免重复定义 #include "Socket.h"
#include <cstdint>
// Socket类
// 作用：
// 封装Linux提供的socket接口
// 管理一个网络通信文件描述符(fd)

class Socket // 创建socket对象
{
public:
    // 构造函数
    // 创建Socket对象时自动调用
    // 负责创建Linux socket
    Socket();

    // 析构函数
    // Socket对象销毁时自动调用
    // 负责释放资源
    ~Socket();

    // 获取socket文件描述符
    // 返回：
    // Linux内核中的socket编号
    int fd() const;

    // 绑定端口号
    void bind(uint16_t port);

    void listen(int backlog);

    int accept();

    // 设置非阻塞
    void setNonBlocking();

    static void setNonBlocking(int fd);

private:
    // socket文件描述符
    //
    // Linux中：
    // 文件、socket、设备都是通过fd访问
    //
    // 例如：
    // fd_=3
    //
    // 表示当前socket在操作系统中的编号
    int fd_;
};

#endif