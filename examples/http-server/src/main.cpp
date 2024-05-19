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
    app.addRoute(GET, "/", RootGETRoute);
    app.addRoute(POST, "/", RootPOSTRoute);

    if (app.connect() == WEBROUTE_ERR) {
        app.printError();
    }

    return 0;
}