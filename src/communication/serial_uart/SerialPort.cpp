#include "SerialPort.h"

SerialPort::SerialPort(asio::io_context& io, const std::string& port, unsigned int baud)
        : io_(io), serial_(io, port)
{
    serial_.set_option(asio::serial_port_base::baud_rate(baud));
    serial_.set_option(asio::serial_port_base::character_size(8));
    serial_.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
    serial_.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));

    doRead();
}

void SerialPort::write(const std::string& data) {
    asio::async_write(serial_, asio::buffer(data),
        [](std::error_code ec, std::size_t length) {
            if (ec) std::cout << "Write error: " << ec.message() << std::endl;
        });
}


void SerialPort::doRead() {
    serial_.async_read_some(asio::buffer(buffer_),
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::string msg(buffer_.data(), length);
                std::cout << "UART Received: " << msg << std::endl;
                doRead();
            } else {
                std::cout << "Read error: " << ec.message() << std::endl;
            }
    });
}