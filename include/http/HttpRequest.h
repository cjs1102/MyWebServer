#pragma once

#include <string>
#include <unordered_map>
using namespace std;

class HttpRequest
{

public:
    HttpRequest();

    // 解析http请求
    bool parse(const string &request);

    // 获取请求方法
    string method() const;

    // 获取路径
    string path() const;

    // 获取HTTP版本
    string version() const;

    // 获取请求头
    string header(const string &key) const;

    // 获取body
    string body() const;

private:
    // 请求方法
    string method_;

    // 请求路径
    string path_;

    // HTTP版本
    string version_;

    // 请求头
    unordered_map<string, string> headers_;

    // 请求正文
    string body_;
};