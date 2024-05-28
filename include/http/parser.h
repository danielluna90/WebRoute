#pragma once

#include "request.h"

enum class HTTPParserStatus {
    OK,
    ERROR,
};

class HTTPParser {
public:
    HTTPParser() = default;
    ~HTTPParser() = default;

    HTTPParserStatus parseHTTPBuffer(HTTPRequest& req, std::string& msgBuffer);

private:
};