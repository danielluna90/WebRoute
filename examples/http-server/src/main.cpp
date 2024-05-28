#include <iostream>

#include <webroute.h>

void RootGETRoute() {
    std::cout << "Hello, Root." << std::endl;
}

void RootPOSTRoute() {
    std::cout << "Hello, Root!" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;

    WebRouteApplication app = WebRouteApplication({ .HTTPS = false });
    app.addRoute(HTTPMethods::GET, "/", RootGETRoute);
    app.addRoute(HTTPMethods::POST, "/", RootPOSTRoute);

    app.addRoute(HTTPMethods::GET, "/:index", RootGETRoute);

    if (app.connect() == WEBROUTE_ERR) {
        app.printError();
    }

    return 0;
}