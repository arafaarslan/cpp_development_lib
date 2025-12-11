#ifndef __TLC_ASIO_SESSION__
#define __TLC_ASIO_SESSION__

#include "TlcAsioUtils.h"

class TlcAsioSession : public std::enable_shared_from_this<TlcAsioSession> {
    public:
        TlcAsioSession(tcp::socket socket, ssl::context& ctx);
        void start();
    private:
        ssl::stream<tcp::socket> stream_;
        std::array<char, 4096> buffer_;

        void gracefulShutdown();
        void doRead();

};

#endif