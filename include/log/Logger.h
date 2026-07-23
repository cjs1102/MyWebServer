#pragma once
#include <string>
#include <mutex>
#include <fstream>

using namespace std;

class Logger
{
public:
    enum Level
    {
        INFO,
        ERROR,
        DEBUG
    };
    static void log(
        Level level,
        const string &messages);

    static void info(
        const string &message);

    static void error(
        const string &message);

    static void debug(
        const string &message);

private:
    static std::mutex mutex_;
    static std::ofstream file_;
};