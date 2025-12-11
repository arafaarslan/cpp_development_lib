#include "TlcAsioSession.h"

TlcAsioSession::TlcAsioSession(tcp::socket socket, ssl::context& ctx)
    : stream_(std::move(socket), ctx) {

    }

void TlcAsioSession::start() {
    auto self = shared_from_this();
    stream_.async_handshake(ssl::stream_base::server,
        [this, self](const asio::error_code& ec) {
            if (!ec) doRead();
            else Logger::Debug("Handshake failed: " + ec.message());
        });
}

void TlcAsioSession::gracefulShutdown() {
    auto self = shared_from_this();
    stream_.async_shutdown([this, self](const asio::error_code& ec) {
        if (ec && ec != asio::error::eof)
            Logger::Error( "Server shutdown error: "  + ec.message());
        else{
            Logger::Debug( "Server shutdown with success: ");
        }
    });
}

void TlcAsioSession::doRead() {
    auto self = shared_from_this();
    stream_.async_read_some(asio::buffer(buffer_),
        [this, self](const asio::error_code& ec, std::size_t n) {
            if (!ec) {
                // echo back
                asio::async_write(stream_, asio::buffer(buffer_, n),
                    [this, self](const asio::error_code& ec2, std::size_t /*m*/) {
                        if (!ec2) doRead();
                        else Logger::Error( "Write error: "  + ec2.message());
                    });
            } else {
                if (ec == asio::error::eof) {
                    gracefulShutdown();
                    return;
                } else {
                    Logger::Error( "Read error: "  + ec.message());
                }
            }
        });
}