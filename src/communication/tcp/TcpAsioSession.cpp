#include "TcpAsioSession.h"

TcpAsioSession::TcpAsioSession(tcp::socket socket)
    : socket_(std::move(socket)){
}

void TcpAsioSession::start() {
    readMessage();
}

void TcpAsioSession::readMessage() {
    auto self = shared_from_this();
    socket_.async_read_some(asio::buffer(data_, BUFFER_SIZE),
        [this, self](std::error_code ec, std::size_t length)
    {
        if (!ec) {
            Logger::Debug("Received: " + std::string(data_, length));
            writeMessage(length);
        }else{
            Logger::Debug("Read error: " + ec.message());
            //End of file mean client closed
            close();
        }
    });
}

void TcpAsioSession::writeMessage(std::size_t length) {
    auto self = shared_from_this();
    asio::async_write(socket_, asio::buffer(data_, length),
        [this, self](std::error_code ec, std::size_t /*length*/)
    {
        if (!ec) {
            readMessage(); // tekrar oku
        }
    });
}

void TcpAsioSession::close() {
    // Graceful shutdown; this overload throws on error, so there is no ignored return value
    try {
        socket_.shutdown(tcp::socket::shutdown_both);
    } catch (const std::system_error& e) {
        // Errors during shutdown are often expected during cleanup (e.g., already closed)
        Logger::Debug("Shutdown error (may be expected): " + std::string(e.what()));
    }

    // Close the socket; again, exceptions signal errors instead of a return value
    try {
        socket_.close();
    } catch (const std::system_error& e) {
        Logger::Debug("Close error (may be expected): " + std::string(e.what()));
    }

    // NOT: Session kapandığında server'ı durdurmamalıyız
    // Server diğer session'ları da yönetmeye devam etmeli
}


