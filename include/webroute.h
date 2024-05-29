#pragma once

#include "http/http.h"
#include "http/parser.h"

#include <string>

#define WEBROUTE_OK 0
#define WEBROUTE_ERR 1

#define WEBROUTE_ERR_SOCKET_FAILED -1
#define WEBROUTE_ERR_PORT_UNAVAILABLE -2

typedef void (*RouteCallback)();

struct AppConfiguration {
    bool HTTPS = false;
    uint32_t port = 80;

    uint16_t maxURILen = 8192;
};

/**
 * @brief WebRoute Application Class.
 *
 */
class WebRouteApplication {
public:
    WebRouteApplication(AppConfiguration config);
    ~WebRouteApplication() = default;

    /**
     * @brief Adds a route to the Application.
     *
     * @param method HTTP Method for the route.
     * @param route String which represents a route.
     * @param cb Callback function for when route is accessed by a client.
     */
    void addRoute(HTTPMethods method, std::string route, RouteCallback cb);

    int connect();

    void printError();

private:
    void handleConnection(int clientSocket);

private:
    AppConfiguration m_Config;

    int m_Socket;
};