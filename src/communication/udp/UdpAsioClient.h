#ifndef __UDP_ASIO_CLIENT__
#define __UDP_ASIO_CLIENT__

#include "UdpAsioUtils.h"

class UdpAsioClient {
    public:
        UdpAsioClient();
        void start();
    private:
        asio::io_context io;
        udp::socket socket;
        asio::steady_timer timer;
        asio::steady_timer closeTimer;
        udp::endpoint server_endpoint;
        std::array<char, BUFFER_SIZE> recv_buf{};
        udp::endpoint sender_endpoint;

        void startSendLoop();
        void sendAsyncMessage();
        void startAsyncReceive();
        void startAutoClose();

};
#endif