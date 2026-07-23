#include "timer/Timer.h"

Timer::Timer(
    int timeout,
    function<void()> cb)

    : callback_(cb),
      canceled_(false)
{

    expireTime_ =
        chrono::steady_clock::now() +
        chrono::seconds(timeout);
}

bool Timer::expired()
{
    if (canceled_)
    {
        return true;
    }
    return chrono::steady_clock::now() >= expireTime_;
}

long long Timer::getRemainingTime()
{

    auto now =
        chrono::steady_clock::now();

    if (now >= expireTime_)
    {
        return 0;
    }

    auto diff =
        chrono::duration_cast<
            chrono::milliseconds>(
            expireTime_ - now);

    return diff.count();
}

void Timer::run()
{

    if (callback_)
    {
        callback_();
    }
}

bool Timer::canceled()
{

    return canceled_;
}

void Timer::cancel()
{
    canceled_ = true;
}