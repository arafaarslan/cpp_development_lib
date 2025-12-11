#ifndef __TCP_ASIO_SERVER__
#define __TCP_ASIO_SERVER__

#include "TcpAsioSession.h"

class TcpAsioServer {
    public:
        TcpAsioServer(asio::io_context& io, short port);
        void start();
        void stop();

    private:
        tcp::acceptor acceptor_;
        asio::io_context& io_;

        void accept();
};

#endif