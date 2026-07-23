#include "log/Logger.h"

#include <iostream>
#include <ctime>

using namespace std;

mutex Logger::mutex_;
ofstream Logger::file_("server.log", ios::app);

void Logger::log(
    Level level,
    const string &message)
{
    lock_guard<mutex> lock(mutex_);

    time_t now = time(nullptr);

    char buffer[64];

    strftime(
        buffer,
        sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        localtime(&now));
    string levelText;

    switch (level)
    {
    case INFO:
        levelText = "INFO";
        break;
    case ERROR:
        levelText = "ERROR";
        break;
    case DEBUG:
        levelText = "DEBUG";
        break;
    }

    string logMessage;

    logMessage = "[" + string(buffer) + "][" + levelText + "]" + message;

    // 终端输出
    cout << logMessage << endl;

    // 文件输出
    if (file_.is_open())
    {
        file_ << logMessage << endl;
        file_.flush();
    }
}

void Logger::info(
    const string &message)
{
    log(INFO, message);
}

void Logger::error(
    const string &message)
{
    log(ERROR, message);
}

void Logger::debug(
    const string &message)
{
    log(DEBUG, message);
}
