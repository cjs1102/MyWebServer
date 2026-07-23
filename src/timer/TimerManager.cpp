#include "timer/TimerManager.h"
#include <climits>
void TimerManager::addTimer(
    shared_ptr<Timer> timer)
{

    timers_.push_back(timer);
}

void TimerManager::handleExpired()
{

    auto it =
        timers_.begin();

    while (it != timers_.end())
    {

        if ((*it)->expired())
        {
            if (!(*it)->canceled())
            {
                (*it)->run();
            }

            it = timers_.erase(it);
        }

        else
        {
            ++it;
        }
    }
}

int TimerManager::getNextTimeout()
{
    if (timers_.empty())
    {
        // 没有Timer
        return -1;
    }

    long long minTime = LLONG_MAX;

    for (auto &timer : timers_)
    {
        long long time =
            timer->getRemainingTime();

        if (time < minTime)
        {
            minTime = time;
        }
    }
    return (int)minTime;
}