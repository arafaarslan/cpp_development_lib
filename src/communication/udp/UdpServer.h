#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "communication/udp/UdpComm.h"
#include "utils/Logger.h"

class UdpServer : public UdpCommunication{

    public:
        int initUdpServer();
        void closeUdpServer();

        bool onMessageReceived(void* data, size_t size) override;
        bool sendMessage(const void* data, size_t size) override;

    private:
        int serverSocket;
        sockaddr_in servAddr;
        sockaddr_in clientAddr;
};

#endif