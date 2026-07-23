#include "thread/Thread.h"

Thread::Thread(function<void()> func)
    : func_(func)
{
}

void Thread::start()
{
    thread_ = thread(func_);
    thread_.detach();
}