#pragma once

#include "timer/Timer.h"

#include <vector>
#include <memory>

using namespace std;

class TimerManager
{

public:
    // 添加定时器
    void addTimer(
        shared_ptr<Timer> timer);

    // 检查过期timer
    void handleExpired();

    // 获取最近Timer时间
    // 返回:
    // 毫秒
    int getNextTimeout();

private:
    vector<shared_ptr<Timer>> timers_;
};