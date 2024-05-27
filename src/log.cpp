#include "log.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

void Log::Print(LogLevel level, std::string fmt, ...) {
    struct timespec ts;
    struct tm* tm;
    char timefmt[64], buf[80];
    std::string levelStr;
    va_list argptr;

    clock_gettime(CLOCK_REALTIME, &ts);
    if ((tm = localtime(&ts.tv_sec)) != NULL) {
        strftime(timefmt, sizeof(timefmt), "%H:%M:%S.%%09u", tm);
        snprintf(buf, sizeof(buf), timefmt, ts.tv_nsec);
    }

    switch (level) {
    case CRITICAL:
        levelStr = "CRITICAL";
        break;
    case ERROR:
        levelStr = "ERROR";
        break;
    case WARN:
        levelStr = "WARN";
        break;
    case INFO:
        levelStr = "INFO";
        break;
    case TRACE:
        levelStr = "TRACE";
        break;
    default:
        levelStr = "UNKNOWN";
        break;
    }

    flockfile(stdout);
    printf("[%s] %8s ", buf, levelStr.c_str());
    va_start(argptr, fmt);
    vprintf(fmt.c_str(), argptr);
    printf("\n");
    funlockfile(stdout);
    va_end(argptr);
    fflush(stdout);
}