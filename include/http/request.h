#pragma once

#include "http.h"

#include <string>
#include <unordered_map>

struct HTTPRequest {
    HTTPMethods method;
    HTTPVersion version;

    std::string uri;
    std::unordered_map<std::string, std::string> headers;

    bool hasBody = false;
    std::string body;

    std::string raw;
};