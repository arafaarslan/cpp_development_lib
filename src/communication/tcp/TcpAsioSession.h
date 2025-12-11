#ifndef __TCP_ASIO_SESSION__
#define __TCP_ASIO_SESSION__

#include "TcpAsioUtils.h"

class TcpAsioSession : public std::enable_shared_from_this<TcpAsioSession>{

    public:
        TcpAsioSession(tcp::socket socket);
        void start();
    private:
        tcp::socket socket_;
        char data_[BUFFER_SIZE];

        void readMessage();
        void writeMessage(std::size_t length);
        void close();

};

#endif