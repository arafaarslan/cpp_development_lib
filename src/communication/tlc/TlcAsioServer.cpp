#include "TlcAsioServer.h"

TlcAsioServer::TlcAsioServer(asio::io_context& io, unsigned short port, const std::string& cert, const std::string& key)
    : io_(io),
    acceptor_(io, tcp::endpoint(tcp::v4(), port)),
    ctx_(ssl::context::tlsv12_server)
{
    // load certificate and key
    ctx_.use_certificate_file(cert, ssl::context::pem);
    ctx_.use_private_key_file(key, ssl::context::pem);

}

void TlcAsioServer::doAccept() {

    if (!acceptor_.is_open()) {
        Logger::Debug("Acceptor closed, stopping accept loop.");
        return;
    }

    acceptor_.async_accept([this](const asio::error_code& ec, tcp::socket sock) {
        if (!ec) {
            std::make_shared<TlcAsioSession>(std::move(sock), ctx_)->start();
            doAccept();
        } else {
            Logger::Error("Accept error: " + ec.message());
        }
    });
}

void TlcAsioServer::stop() {
    asio::post(io_, [this]() {
        // Close acceptor - errors are often expected during shutdown
        try {
            acceptor_.close();
        } catch (const std::system_error& e) {
            // Errors during close are often expected (e.g., already closed)
            Logger::Error("Accept close error (may be expected): " + std::string(e.what()));
        }
    });
}
