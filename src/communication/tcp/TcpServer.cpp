#include "communication/tcp/TcpServer.h"

int TcpServer::initTcpServer(){

    serverSocket = -1;
    clientSocket = -1; 

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; // listen on localhost
    servaddr.sin_port = htons(PORT);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        close(serverSocket);
        return 1;
    }

    if (bind(serverSocket, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 3) < 0) {
        perror("listen");
        return 1;
    }

    Logger::Debug("Server listening on port : " + std::to_string(PORT));
    return 0;
}

int TcpServer::acceptConnectionOnTcpServer() {

    socklen_t len = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &len);

    if (clientSocket < 0) {
        perror("accept");
        return 1;
    }

    Logger::Debug("Client connected!");
    return 0;
}

void TcpServer::closeTcpServer() {
    if (clientSocket >= 0) {
        Logger::Debug("close :  server client socket !!!");
        close(clientSocket);
    }

    if (serverSocket >= 0) {
        Logger::Debug("close :  server socket !!!");
        close(serverSocket);
    }
}

bool TcpServer::onMessageReceived(void* data, size_t size) {
    ssize_t n = recv(clientSocket, data, size, 0);
    return n > 0;
}

bool TcpServer::sendMessage(const void* data, size_t size) {
    ssize_t n = send(clientSocket, data, size, 0);
    return n == (ssize_t)size;
}