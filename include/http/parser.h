#pragma once

#include "http/http.h"
#include "request.h"

enum class HTTPParserState {
    STATUS_LINE,
    HEADERS,
    BODY,
    DONE,
    ERROR
};

class HTTPParser {
public:
    HTTPParser() = default;
    ~HTTPParser() = default;

    void parseHTTPBuffer(HTTPRequest& req, std::string& msgBuffer);
    const HTTPParserState getState() { return m_State; }

private:
    HTTPMethods getHTTPMethodFromStr(const std::string& methodStr);

private:
    HTTPParserState m_State = HTTPParserState::STATUS_LINE;
};