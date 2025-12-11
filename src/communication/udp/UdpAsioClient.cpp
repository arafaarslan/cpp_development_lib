#include "UdpAsioClient.h"

UdpAsioClient::UdpAsioClient()
    : socket(io, udp::endpoint(udp::v4(), 0)),
    timer(io),
    closeTimer(io),
    server_endpoint(asio::ip::make_address("127.0.0.1"), 5555)
{}

void UdpAsioClient::start() {
    startSendLoop();
    startAsyncReceive();
    startAutoClose();
    io.run();
}

void UdpAsioClient::startSendLoop() {
    timer.expires_after(std::chrono::seconds(1));

    timer.async_wait([this](std::error_code ec) {
        if (!ec) {
            sendAsyncMessage();   // async send
            startSendLoop();      // schedule next tick
        }
    });
}

void UdpAsioClient::sendAsyncMessage() {
    std::string message = "Hello UDP Server!";

    socket.async_send_to(
        asio::buffer(message),
        server_endpoint,
        [this](std::error_code ec, std::size_t /*bytes_sent*/) {
            if (!ec) {
                Logger::Debug("[CLIENT] Message sent");
                
            } else {
                Logger::Debug("[CLIENT] Send error: " +  ec.message() );
            }
        }
    );
}

void UdpAsioClient::startAsyncReceive() {
    socket.async_receive_from(
        asio::buffer(recv_buf),
        sender_endpoint,
        [this](std::error_code ec, std::size_t len) {
            if (!ec) {
                Logger::Debug("[CLIENT] Reply: " + std::string(recv_buf.data(), len));
            } else {
                Logger::Debug("[CLIENT] Receive error: " + ec.message() );
            }
            startAsyncReceive();
        }
    );
}

void UdpAsioClient::startAutoClose() {
    closeTimer.expires_after(std::chrono::seconds(5));

    closeTimer.async_wait([this](const asio::error_code& ec)
    {
        if (!ec) {
            Logger::Debug("Closing connection after 5 seconds...");
            // Close socket - errors are often expected during cleanup
            try {
                socket.close();
            } catch (const std::system_error& e) {
                Logger::Debug("Close error (may be expected): " + std::string(e.what()));
            }
            io.stop();
        }
    });

}