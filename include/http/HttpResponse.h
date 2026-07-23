#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class HttpResponse
{

public:
    // 构造函数
    HttpResponse();

    // 设置状态码
    void setStatusCode(int code);

    // 设置响应内容
    void setBody(const string &body);

    // 读取文件
    bool readFile(const string &filename);

    // 生成完整HTTP响应字符串
    string makeResponse();

    string statusMessage();

    // 设置响应头

    void setHeader(
        const string &key,
        const string &value);

private:
    int status_code_;

    string body_;

    unordered_map<string, string> headers_;

private:
    // 根据文件类型获取MIME
    string getMimeType(const string &filename);
};