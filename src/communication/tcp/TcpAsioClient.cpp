#include "TcpAsioClient.h"

TcpAsioClient::TcpAsioClient()
    : resolver(io), socket(io),sendTimer(io),closeTimer(io) {

}

void TcpAsioClient::startAsyncRead() {
    socket.async_read_some(
        asio::buffer(readBuffer),
        [this](const asio::error_code& ec, std::size_t len)
        {
            if (!ec) {
                Logger::Debug("Reply: " + std::string(readBuffer.data(), len));
                startAsyncRead();
            } else {
                Logger::Debug("Read error: " + ec.message());
            }
        });
}

void TcpAsioClient::sendMessage(){

    const std::string msg = "Hello from client!";

    asio::async_write(
        socket,
        asio::buffer(msg),
        [this](const asio::error_code& ec, std::size_t /*len*/) 
        {
            if (!ec) {
                //Logger::Debug("Sending ");
            } else {
                Logger::Debug("Send error: " + ec.message());
            }
        });
}

void TcpAsioClient::startPeriodicSend()
{
    sendTimer.expires_after(std::chrono::seconds(1));

    sendTimer.async_wait([this](const asio::error_code& ec)
    {
        if (!ec) {
            sendMessage();     // mesaj gönder
            startPeriodicSend(); // Sonraki periyot için kendini kur
        }
    });
}

void TcpAsioClient::runClient(){     
    auto endpoints = resolver.resolve("127.0.0.1", "5555");

    asio::connect(socket, endpoints);

    Logger::Debug("Connected to server");

    startAsyncRead(); 
    startPeriodicSend();
    startAutoClose(); 

    io.run();
    
}

void TcpAsioClient::startAutoClose() {
    closeTimer.expires_after(std::chrono::seconds(5));

    closeTimer.async_wait([this](const asio::error_code& ec)
    {
        if (!ec) {
            Logger::Debug("Closing connection after 5 seconds...");
            asio::error_code ignored;
            try {
                socket.close();
            } catch (const std::system_error& e) {
                Logger::Debug("Close error (may be expected): " + std::string(e.what()));
            }

            io.stop();
        }
    });

}