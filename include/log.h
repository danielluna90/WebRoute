#pragma once

#include <string>

enum class LogLevel {
    TRACE,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
};

class Log {
public:
    static void Print(LogLevel level, std::string fmt, ...);
};