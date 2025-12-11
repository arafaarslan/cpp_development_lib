#ifndef __TCP_ASIO_CLIENT__
#define __TCP_ASIO_CLIENT__

#include "TcpAsioUtils.h"

class TcpAsioClient{
    public:
        TcpAsioClient();
        void runClient();
        void startAutoClose();

    private:
        asio::io_context io;
        tcp::resolver resolver;
        tcp::socket socket;
        asio::steady_timer sendTimer,closeTimer;
        std::array<char, BUFFER_SIZE> readBuffer;

        void startAsyncRead();
        void sendMessage();
        void startPeriodicSend();

};  

#endif
