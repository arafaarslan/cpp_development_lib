#include "UdpAsioServer.h"

UdpAsioServer::UdpAsioServer(asio::io_context& io, int port)
    : socket_(io, udp::endpoint(udp::v4(), port)) {
}

void UdpAsioServer::start(){
    startReceive();
}

void UdpAsioServer::stop() {
    socket_.close();
}

void UdpAsioServer::startReceive() {
    socket_.async_receive_from(
        asio::buffer(data_),
        remote_endpoint_,
        [this](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec) {
                std::string msg(data_.data(), bytes_recvd);
                Logger::Debug("Server : Received: " + msg +  " from " + remote_endpoint_.address().to_string() +  ":" + std::to_string(remote_endpoint_.port()));
                
                writeMessage();

                startReceive(); // tekrar dinle
            }
        }
    );
}

void UdpAsioServer::writeMessage(){
    // İstersen cevap gönder
    std::string reply = "Message received";
    socket_.async_send_to(
        asio::buffer(reply),
        remote_endpoint_,
        [](std::error_code /*ec*/, std::size_t /*bytes_sent*/) {}
    );
}