#include "net/Epoll.h"

#include <unistd.h>

#include <iostream>

using namespace std;

Epoll::Epoll()
{
    epoll_fd_ = epoll_create1(0);

    events_.resize(1024);
}

Epoll::~Epoll()
{
    close(epoll_fd_);
}

void Epoll::addFd(int fd, bool edgeTrigger)
{
    struct epoll_event event;

    event.events = EPOLLIN;

    if (edgeTrigger)
    {
        event.events |= EPOLLET;
    }
    event.data.fd = fd;
    epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
}

void Epoll::modifyFd(
    int fd,
    uint32_t events)
{
    epoll_event event;

    event.events = events;

    event.data.fd = fd;

    epoll_ctl(
        epoll_fd_,
        EPOLL_CTL_MOD,
        fd,
        &event);
}

void Epoll::removeFd(int fd)
{
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
}

vector<epoll_event> Epoll::wait(int timeout)
{
    vector<epoll_event> events(1024);

    int n = epoll_wait(
        epoll_fd_, events.data(),
        events.size(), timeout);

    if (n < 0)
    {
        return {};
    }

    events.resize(n);

    return events;
}