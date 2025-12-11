
#include "TcpAsioServer.h"

TcpAsioServer::TcpAsioServer(asio::io_context& io, short port)
    : io_(io), acceptor_(io, tcp::endpoint(tcp::v4(), port)){
    
}

void TcpAsioServer::start(){
    accept();
}

void TcpAsioServer::stop() {
    asio::post(io_, [this]() {
        // Close acceptor - errors are often expected during shutdown
        try {
            acceptor_.close();
        } catch (const std::system_error& e) {
            // Errors during close are often expected (e.g., already closed)
            Logger::Debug("Acceptor close error (may be expected): " + std::string(e.what()));
        }
        io_.stop();
    });
}

void TcpAsioServer::accept() {
    if (!acceptor_.is_open()) {
        Logger::Debug("Acceptor closed, stopping accept loop.");
        return;
    }

    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
    {
        if (!ec) {
            Logger::Debug("Client connected!");
            auto session = std::make_shared<TcpAsioSession>(std::move(socket));
            session->start();
        }
        accept(); // tekrar yeni client kabul et
    });
}