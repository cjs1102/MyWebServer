#pragma once

#include <sys/epoll.h>

#include <vector>

using namespace std;

class Epoll
{
public:
    Epoll();

    ~Epoll();

    // 添加文件描述符到epoll中
    void addFd(
        int fd,
        bool edgeTrigger = true);

    void modifyFd(
        int fd,
        uint32_t events);

    // 从epoll中删除文件描述符
    void removeFd(int fd);

    // 等待事件发生
    vector<epoll_event> wait(int timeout);

private:
    int epoll_fd_;

    vector<epoll_event> events_;
};