#pragma once

#include <string>
#include <unordered_map>
using namespace std;

class HttpRequest
{

public:
    HttpRequest();

    bool parse(const string &request);

private:
    // 请求方法
    string method_;

    // 请求路径
    string path_;

    // HTTP版本
    string version_;

    // 请求头
    unordered_map<string, string> headers_;
};