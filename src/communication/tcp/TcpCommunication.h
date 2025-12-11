#ifndef _TCP_COMM__
#define _TCP_COMM__

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

//Make the listener socket non-blocking
#include <fcntl.h>


#define PORT 56100 
#define BUFFER_SIZE 1024

class TcpCommunication{

    public:
        virtual ~TcpCommunication() = default;

        virtual bool onMessageReceived(void* data, size_t size) = 0;
        virtual bool sendMessage(const void* data, size_t size) = 0;

    private :

};

#endif
