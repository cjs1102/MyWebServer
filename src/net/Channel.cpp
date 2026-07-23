#include "net/Channel.h"

Channel::Channel(int fd)
    : fd_(fd), enabled_(true)
{
}

int Channel::fd() const
{
    return fd_;
}

void Channel::setReadCallback(function<void()> cb)
{
    readCallback_ = cb;
}

void Channel::setWriteCallback(
    function<void()> cb)
{

    writeCallback_ = cb;
}

void Channel::handleEvent()
{
    if (!enabled_)
    {
        return;
    }
    if (readCallback_)
    {
        readCallback_();
    }
}

void Channel::handleWrite()
{

    if (writeCallback_)
    {
        writeCallback_();
    }
}

void Channel::disable()
{
    enabled_ = false;
}

bool Channel::enabled() const
{
    return enabled_;
}