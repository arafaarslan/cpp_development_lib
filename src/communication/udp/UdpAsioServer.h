#ifndef __UDP_ASIO_SERVER__
#define __UDP_ASIO_SERVER__

#include "UdpAsioUtils.h"

class UdpAsioServer {
    public:
        UdpAsioServer(asio::io_context& io, int port);
        void start();
        void stop();
    private:
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, BUFFER_SIZE> data_;

        void startReceive();
        void writeMessage();

};

#endif