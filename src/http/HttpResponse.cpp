#include "http/HttpResponse.h"
#include <sstream>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

HttpResponse::HttpResponse()
{
    status_code_ = 200;

    headers_["Content-Type"] = "text/html";
}

bool HttpResponse::readFile(const string &filename)
{
    struct stat info;

    if (stat(filename.c_str(), &info) != 0)
    {
        status_code_ = 404;
        body_ = "<h1>404 Not Found</h1>";
        return false;
    }
    // 判断是不是普通文件
    if (!S_ISREG(info.st_mode))
    {
        status_code_ = 404;
        body_ = "<h1>404 Not Found</h1>";
        return false;
    }
    ifstream file(filename);

    string content((istreambuf_iterator<char>(file)),
                   istreambuf_iterator<char>());
    body_ = content;
    // 设置MIME类型
    headers_["Content-Type"] = getMimeType(filename);

    file.close();

    return true;
}
void HttpResponse::setStatusCode(int code)
{
    status_code_ = code;
}

void HttpResponse::setBody(const string &body)
{
    body_ = body;
}

string HttpResponse::statusMessage()
{
    switch (status_code_)
    {
    case 200:
        return "OK";
    case 400:
        return "Bad Request";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 500:
        return "Internal Server Error";
    default:
        return "Unknown";
    }
}

void HttpResponse::setHeader(
    const string &key,
    const string &value)
{

    headers_[key] = value;
}

string HttpResponse::makeResponse()
{
    stringstream response;

    // 状态行
    response
        << "HTTP/1.1 "
        << status_code_
        << " "
        << statusMessage()
        << "\r\n";

    // 响应头
    for (const auto &header : headers_)
    {

        response
            << header.first
            << ": "
            << header.second
            << "\r\n";
    }

    response
        << "Content-Length: "
        << body_.size()
        << "\r\n";

    // 空行
    response
        << "\r\n";

    // 正文

    response
        << body_;

    return response.str();
}

string HttpResponse::getMimeType(const string &filename)
{

    size_t pos = filename.find_last_of('.');

    if (pos == string::npos)
    {
        return "text/plain";
    }

    string ext = filename.substr(pos);

    if (ext == ".html" || ext == ".htm")
    {
        return "text/html";
    }

    if (ext == ".css")
    {
        return "text/css";
    }

    if (ext == ".js")
    {
        return "application/javascript";
    }

    if (ext == ".png")
    {
        return "image/png";
    }

    if (ext == ".jpg" || ext == ".jpeg")
    {
        return "image/jpeg";
    }

    if (ext == ".gif")
    {
        return "image/gif";
    }

    if (ext == ".ico")
    {
        return "image/x-icon";
    }

    if (ext == ".txt")
    {
        return "text/plain";
    }

    return "application/octet-stream";
}