#include "TcpClient.h"


int TcpClient::initTcpClient(){

    clientSocket = -1;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    // 1. Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        Logger::Debug("Socket creation error");
        return 1;
    }

    // 2. Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0) {
        Logger::Debug("Invalid address / Address not supported");
        return 1;
    }

    // 3. Connect to server
    if (connect(clientSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        Logger::Debug("Connection Failed");
        return 1;
    }

    int flags = fcntl(clientSocket, F_GETFL, 0);
    if(flags == -1){
        Logger::Debug("fcntl(F_GETFL) failed");
        return 1;
    }

    int resp = fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
    if(resp == -1 ){
        Logger::Debug("fcntl(F_SETFL) failed");
        return 1;
    }

    return 0;

}


void TcpClient::closeTcpClient(){
    if (clientSocket >= 0) {
        Logger::Debug("close :  client socket !!!");
        close(clientSocket);
    }
}


bool TcpClient::onMessageReceived(void* data, size_t size) {
    ssize_t n = recv(clientSocket, data, size, 0);
    return n > 0;
}

bool TcpClient::sendMessage(const void* data, size_t size) {
    ssize_t n = send(clientSocket, data, size, 0);
    return n == (ssize_t)size;
}