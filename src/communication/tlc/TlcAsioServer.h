#ifndef __TLC_ASIO_SERVER__
#define __TLC_ASIO_SERVER__

#include "TlcAsioSession.h"

class TlcAsioServer {
    public:
        TlcAsioServer(asio::io_context& io, unsigned short port, const std::string& cert, const std::string& key);
        void doAccept();
        void stop();
    private:
        asio::io_context& io_;
        tcp::acceptor acceptor_;
        ssl::context ctx_;
};
#endif