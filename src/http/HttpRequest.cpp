#include "http/HttpRequest.h"
#include <sstream>

HttpRequest::HttpRequest()
{
}

bool HttpRequest::parse(const string &request)
{
    stringstream ss(request);

    // 解析请求行
    string line;

    getline(ss, line);

    stringstream requestLine(line);

    if (!(requestLine >> method_ >> path_ >> version_))
    {
        return false;
    }

    // 解析Header
    while (getline(ss, line))
    {
        // 遇到空行
        // 表示header结束
        if (line == "\r" || line.empty())
        {
            break;
        }
        size_t pos = line.find(':');

        if (pos != string::npos)
        {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);

            // 去除前面的空格
            if (!value.empty() && value[0] == ' ')
            {
                value.erase(0, 1);
            }
            // 去掉\r
            if (!value.empty() && value.back() == '\r')
            {
                value.pop_back();
            }

            headers_[key] = value;
        }
    }

    // 解析body
    body_ = string(
        (istreambuf_iterator<char>(ss)),
        istreambuf_iterator<char>());

    return true;
}

string HttpRequest::method() const
{
    return method_;
}

string HttpRequest::path() const
{
    return path_;
}

string HttpRequest::version() const
{
    return version_;
}

string HttpRequest::header(const string &key) const
{
    auto it = headers_.find(key);

    if (it != headers_.end())
    {
        return it->second;
    }
    return ""; // 返回空字符串表示未找到该头部
}

string HttpRequest::body() const
{
    return body_;
}
