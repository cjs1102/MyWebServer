#include "http/HttpRequest.h"
#include <sstream>

HttpRequest::HttpRequest()
{
}

bool HttpRequest::parse(const string &request)
{
    stringstream ss(request);

    ss >> method_;
    ss >> path_;
    ss >> version_;

    return true;
}