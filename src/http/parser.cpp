#include "http/parser.h"

#include <sstream>

#include "http/http.h"
#include "log.h"

int const HTTP_STATUS_LINE_VALUE_COUNT = 3;

HTTPParserStatus HTTPParser::parseHTTPBuffer(HTTPRequest& req, std::string& msgBuffer) {
    size_t nextLinePos;
    std::string nextLine;

    while ((nextLinePos = msgBuffer.find("\r\n")) != std::string::npos || req.hasBody) {
        nextLine = msgBuffer.substr(0, nextLinePos);

        if (req.uri.empty()) {
            // Parse Status Line
            std::stringstream statusLine(nextLine);
            std::string s;

            Log::Print(LogLevel::INFO, "Status Line: %s", nextLine.c_str());

            int i = 0;
            while (std::getline(statusLine, s, ' ')) {
                if (i == 0) {
                    // HTTP Method
                    req.method = HTTPMethods::GET;
                } else if (i == 1) {
                    // URI
                    req.uri = s;

                } else if (i == 2) {
                    // HTTP Version
                    req.version = HTTPVersion::HTTP_VERSION_1_1;
                } else {
                    Log::Print(LogLevel::CRITICAL, "HTTP status line contains more than 3 values.");

                    return HTTPParserStatus::ERROR;
                }

                i++;
            }

            if (i != HTTP_STATUS_LINE_VALUE_COUNT) {
                Log::Print(LogLevel::CRITICAL, "HTTP status line contains less than 3 values.");

                return HTTPParserStatus::ERROR;
            }

            msgBuffer.erase(0, nextLinePos + 2);
            continue;
        }

        if (nextLine.empty()) {
            msgBuffer.erase(0, nextLinePos + 2);
            req.hasBody = true;
            continue;
        }

        if (req.hasBody) {
            Log::Print(LogLevel::INFO, "Message Body: %s", nextLine.c_str());
        } else {
            Log::Print(LogLevel::INFO, "Header Field: %s", nextLine.c_str());

            msgBuffer.erase(0, nextLinePos + 2);
        }
    }

    return HTTPParserStatus::OK;
}