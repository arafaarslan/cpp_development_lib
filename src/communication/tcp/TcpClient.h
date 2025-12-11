#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "communication/tcp/TcpCommunication.h"
#include "utils/Logger.h"

class TcpClient : public TcpCommunication{

    public:
        int initTcpClient();
        void closeTcpClient();

        bool onMessageReceived(void* data, size_t size) override;
        bool sendMessage(const void* data, size_t size) override;

    private:
        int clientSocket;
        sockaddr_in servAddr;

};

#endif