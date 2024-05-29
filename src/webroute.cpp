#include "webroute.h"

#include "http/parser.h"
#include "log.h"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

WebRouteApplication::WebRouteApplication(AppConfiguration config)
    : m_Config(config) {
    if (m_Config.HTTPS) {
        if (m_Config.port == 80)
            m_Config.port = 443;
    }
}

void WebRouteApplication::addRoute(HTTPMethods method, std::string route, RouteCallback cb) {
    Log::Print(LogLevel::INFO, "Adding Route: %s", route.c_str());

    cb();
}

int WebRouteApplication::connect() {
    Log::Print(LogLevel::INFO, "Attempted to Connect on Port %u", m_Config.port);

    int yes = 1;

    m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_Socket < 0) {
        Log::Print(LogLevel::CRITICAL, "Socket socket() Failed.");

        return WEBROUTE_ERR_SOCKET_FAILED;
    }

    if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        Log::Print(LogLevel::CRITICAL, "Socket setsockopt() Failed.");
        close(m_Socket);

        return WEBROUTE_ERR_SOCKET_FAILED;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_Config.port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_Socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        Log::Print(LogLevel::CRITICAL, "Socket bind() Failed.");
        close(m_Socket);

        return WEBROUTE_ERR_PORT_UNAVAILABLE;
    }

    if (listen(m_Socket, 5) == -1) {
        Log::Print(LogLevel::CRITICAL, "Socket listen() Failed.");
        close(m_Socket);

        return WEBROUTE_ERR_PORT_UNAVAILABLE;
    }

    int clientSocket;
    struct sockaddr_storage* clientAddr;
    socklen_t sockAddrLen = sizeof(struct sockaddr_storage);

    bool acceptingConnections = true;
    while (acceptingConnections) {
        clientAddr = (struct sockaddr_storage*)calloc(1, sockAddrLen);

        if ((clientSocket = accept(m_Socket, (struct sockaddr*)clientAddr, &sockAddrLen)) == -1) {
            Log::Print(LogLevel::CRITICAL, "Could not accept() connection");
            free(clientAddr);

            continue;
        }

        std::thread workerThread([&] { handleConnection(clientSocket); });
        workerThread.detach();
    }

    return WEBROUTE_OK;
}

void WebRouteApplication::printError() {
}

void WebRouteApplication::handleConnection(int clientSocket) {
    bool quit = true;

    int sizeRecv, msgIndex = 0;
    const uint16_t CHUNK_SIZE = 8192;
    char buffer[CHUNK_SIZE];

    std::string msg;

    HTTPParser parser;
    HTTPRequest req;

    while (true) {
        memset(buffer, 0, CHUNK_SIZE);

        if ((sizeRecv = recv(clientSocket, buffer, CHUNK_SIZE, 0)) >= 0) {
            if (sizeRecv == 0)
                break;

            msg.append(buffer, sizeRecv);

            Log::Print(LogLevel::INFO, "Recieved HTTP Request: %s", msg.c_str());

            parser.parseHTTPBuffer(req, msg);

            if (parser.getState() == HTTPParserState::DONE)
                break;
        } else {
            Log::Print(LogLevel::CRITICAL, "recv() failed");

            break;
        }
    }

    close(clientSocket);
}