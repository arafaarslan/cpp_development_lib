#ifndef __SERIAL_PORT__
#define __SERIAL_PORT__

#include <asio.hpp>
#include <iostream>

class SerialPort {
public:
    SerialPort(asio::io_context& io, const std::string& port, unsigned int baud);
    void write(const std::string& data);

private:
    asio::io_context& io_;
    asio::serial_port serial_;
    std::array<char, 256> buffer_;


    void doRead();
};

#endif