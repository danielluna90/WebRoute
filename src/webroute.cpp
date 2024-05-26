#include "webroute.h"

#include "log.h"

WebRouteApplication::WebRouteApplication(AppConfiguration config)
    : m_Config(config) {
    if (m_Config.HTTPS) {
    }
}

void WebRouteApplication::addRoute(HTTPMethods method, std::string route, RouteCallback cb) {
    cb();
}

int WebRouteApplication::connect() {
    Log::Print(TRACE, "Hello %d", 5);

    return WEBROUTE_OK;
}

void WebRouteApplication::printError() {
}