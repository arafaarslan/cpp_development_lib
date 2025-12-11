#include "UdpServer.h"

int UdpServer::initUdpServer() {

    serverSocket = -1;

    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(CLIENT_PORT);

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(SERVER_PORT);

    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        Logger::Error("init_udp_server :: socket failed");
        return 1;
    }

    if (bind(serverSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        Logger::Error("init_udp_server :: bind failed");
        close(serverSocket);
        return 1;
    }

    Logger::Debug("Server listening on port " + std::to_string(SERVER_PORT));
    return 0;
}


void UdpServer::closeUdpServer() {
    if (serverSocket >= 0) {
        Logger::Debug("close :  server socket !!!");
        close(serverSocket);
    }
}


bool UdpServer::onMessageReceived(void* data, size_t size) {
    socklen_t len = sizeof(clientAddr);

    ssize_t n = recvfrom(
            serverSocket,
            data,
            size,
            0,//0: blocking, MSG_DONTWAIT : non blocking
            (struct sockaddr*)&clientAddr,
            &len);

    return n == sizeof(data);
}

bool UdpServer::sendMessage(const void* data, size_t size) {
    socklen_t len = sizeof(clientAddr);
    ssize_t n = sendto(
        serverSocket,
        data,
        size,
        0,
        (struct sockaddr*)&clientAddr,
        len);

    return n == sizeof(data);
}