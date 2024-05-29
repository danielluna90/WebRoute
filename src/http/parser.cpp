#include "http/parser.h"

#include <sstream>
#include <string>

#include "http/http.h"
#include "log.h"

const int HTTP_STATUS_LINE_VALUE_COUNT = 3;

const int HTTP_LINE_TERMINATOR_LENGTH = 2;
static const char HTTP_DELIMITER[] = "\r\n";
static const char HTTP_HEADER_DELIMITER = ':';

void HTTPParser::parseHTTPBuffer(HTTPRequest& req, std::string& msgBuffer) {
    size_t nextLinePos;
    std::string nextLine;

    while ((nextLinePos = msgBuffer.find(HTTP_DELIMITER)) != std::string::npos || m_State == HTTPParserState::BODY) {
        nextLine = msgBuffer.substr(0, nextLinePos);

        if (m_State == HTTPParserState::STATUS_LINE) {
            // Parse Status Line
            std::stringstream statusLine(nextLine);
            std::string s;

            int i = 0;
            while (std::getline(statusLine, s, ' ')) {
                if (i == 0) {
                    // HTTP Method
                    req.method = getHTTPMethodFromStr(s);
                } else if (i == 1) {
                    // URI
                    req.uri = s;
                } else if (i == 2) {
                    // HTTP Version
                    req.version = HTTPVersion::HTTP_VERSION_1_1;
                } else {
                    Log::Print(LogLevel::CRITICAL, "HTTP status line contains more than 3 values.");

                    m_State = HTTPParserState::ERROR;
                    return;
                }

                i++;
            }

            if (i != HTTP_STATUS_LINE_VALUE_COUNT) {
                Log::Print(LogLevel::CRITICAL, "HTTP status line contains less than 3 values.");

                m_State = HTTPParserState::ERROR;
                return;
            }

            m_State = HTTPParserState::HEADERS;
        } else if (m_State == HTTPParserState::HEADERS) {
            if (nextLine.empty()) {
                m_State = HTTPParserState::BODY;
                continue;
            }

            std::stringstream header(nextLine);
            std::string fieldName;
            std::string fieldVal;

            std::getline(header, fieldName, HTTP_HEADER_DELIMITER);
            std::getline(header, fieldVal, HTTP_HEADER_DELIMITER);

            while (fieldVal[0] == ' ')
                fieldVal.erase(0, 1);

            while (fieldVal[fieldVal.size() - 1] == ' ')
                fieldVal.pop_back();

            req.headers[fieldName] = fieldVal;
        } else if (m_State == HTTPParserState::BODY) {
            if (req.headers.find("Content-Length") == req.headers.end()) {
                m_State = HTTPParserState::DONE;
                return;
            }

            Log::Print(LogLevel::INFO, "Message Body: %s", nextLine.c_str());
            req.body.append(msgBuffer);

            break;
        }

        if (m_State != HTTPParserState::BODY)
            msgBuffer.erase(0, nextLinePos + HTTP_LINE_TERMINATOR_LENGTH);
    }
}

HTTPMethods HTTPParser::getHTTPMethodFromStr(const std::string& methodStr) {
    if (methodStr == "GET") {
        return HTTPMethods::GET;
    }

    return HTTPMethods::UNKNOWN;
}