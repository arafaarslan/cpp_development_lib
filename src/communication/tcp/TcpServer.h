#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "communication/tcp/TcpCommunication.h"
#include "utils/Logger.h"

class TcpServer : public TcpCommunication{

    public:
        int initTcpServer();
        int acceptConnectionOnTcpServer();
        void closeTcpServer();

        bool onMessageReceived(void* data, size_t size) override;
        bool sendMessage(const void* data, size_t size) override;

    private:
        int serverSocket;
        int clientSocket;
        sockaddr_in clientAddr;
        sockaddr_in servaddr;

};

#endif