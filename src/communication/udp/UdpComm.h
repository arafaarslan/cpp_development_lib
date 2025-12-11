#ifndef _UDP_COMM__
#define _UDP_COMM__

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define SERVER_PORT 56100
#define CLIENT_PORT 56101
#define BUFFER_SIZE 1024

class UdpCommunication{

    public:
        virtual ~UdpCommunication() = default;

        virtual bool onMessageReceived(void* data, size_t size) = 0;
        virtual bool sendMessage(const void* data, size_t size) = 0;

    private :

};

#endif
