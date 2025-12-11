#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "communication/udp/UdpComm.h"
#include "utils/Logger.h"

class UdpClient  : public UdpCommunication{
    public:

        int initUdpClient();
        void closeUdpClient();

        bool onMessageReceived(void* data, size_t size) override;
        bool sendMessage(const void* data, size_t size) override;

    private:
        int clientSocket;
        sockaddr_in cliAddr;
        sockaddr_in serverAddr;

};

#endif