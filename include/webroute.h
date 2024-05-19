#pragma once

#include <string>

#define WEBROUTE_OK 0
#define WEBROUTE_ERR 1

#define WEBROUTE_ERR_PORT_UNAVAILABLE -1

typedef void (*RouteCallback)();

enum HTTPMethods {
    POST,
    GET,
    PUT,
    PATCH,
    DELETE
};

struct AppConfiguration {
    bool HTTPS = false;
};

class WebRouteApplication {
public:
    WebRouteApplication(AppConfiguration config);
    ~WebRouteApplication() = default;

    void addRoute(HTTPMethods method, std::string route, RouteCallback cb);

    int connect();

    void printError();

private:
    AppConfiguration m_Config;
};