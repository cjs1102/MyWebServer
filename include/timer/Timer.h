#pragma once

#include <functional>
#include <chrono>

using namespace std;

// 定时器
//
// 保存：
//
// 1. 到期时间
// 2. 超时回调
//

class Timer
{

public:
    Timer(
        int timeout,
        function<void()> cb);

    // 判断是否超时
    bool expired();

    // 执行回调
    void run();

    // 获取剩余时间(ms)
    long long getRemainingTime();

    // 取消Timer
    void cancel();

    // 判断是否取消
    bool canceled();

private:
    // 到期时间
    chrono::steady_clock::time_point expireTime_;

    // 回调函数
    function<void()> callback_;

    // 是否取消
    bool canceled_;
};