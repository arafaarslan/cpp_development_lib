#include "UdpClient.h"

int UdpClient::initUdpClient() {
    
    clientSocket = -1;
    
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(SERVER_PORT);

    memset(&cliAddr, 0, sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    cliAddr.sin_port = htons(CLIENT_PORT);

    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        Logger::Error("init_udp_client :: Socket creation error");
        return 1;
    }

    if (bind(clientSocket, (struct sockaddr*)&cliAddr, sizeof(cliAddr)) < 0) {
        Logger::Error("init_udp_client :: client bind failed");
        close(clientSocket);
        return 1;
    }

    Logger::Debug("Client listening on port " + std::to_string(CLIENT_PORT));
    return 0;
}


void UdpClient::closeUdpClient() {
    if (clientSocket >= 0) {
        Logger::Debug("close :  client socket !!!");
        close(clientSocket);
    }
}

bool UdpClient::onMessageReceived(void* data, size_t size) {
    socklen_t server_add_len = sizeof(serverAddr);
    ssize_t n = recvfrom(
        clientSocket,
        data,
        size,
        MSG_DONTWAIT,//0: blocking, MSG_DONTWAIT : non blocking
        (struct sockaddr*)&cliAddr,
        &server_add_len
    );
    return n == sizeof(data);
}

bool UdpClient::sendMessage(const void* data, size_t size) {
    ssize_t n = sendto(
        clientSocket,
        data,
        size,
        0,  // normally 0 for sendto
        (struct sockaddr*)&serverAddr,
        sizeof(serverAddr)
    );

    return n == sizeof(data);
}