#pragma once

#include <thread>
#include <functional>

using namespace std;

class Thread
{

public:
    Thread(function<void()> func);

    void start();

private:
    thread thread_;
    function<void()> func_;
};