#include "TlcAsioClient.h"

TlcAsioClient::TlcAsioClient(){
}


void TlcAsioClient::runClient(){     

    ssl::context ctx(ssl::context::tlsv12_client);
    // For testing with self-signed cert, either:
    // 1) load the server.crt as trusted root:
    ctx.load_verify_file("tls_certificate/server.crt");
    ctx.set_verify_mode(ssl::verify_peer);

    // OR (not recommended for production) skip verification:
    // ctx.set_verify_mode(ssl::verify_none);

    ssl::stream<tcp::socket> ssl_socket(io, ctx);

    // resolve + connect
    tcp::resolver resolver(io);
    auto endpoints = resolver.resolve("127.0.0.1", "5556");
    asio::connect(ssl_socket.lowest_layer(), endpoints);

    // perform TLS handshake
    ssl_socket.handshake(ssl::stream_base::client);

    // send message
    std::string msg = "Hello secure world!";
    asio::write(ssl_socket, asio::buffer(msg));

    // read echo
    std::array<char, 4096> buf;
    size_t n = ssl_socket.read_some(asio::buffer(buf));
    Logger::Debug( "Reply: " + std::string(buf.data(), n) );

    // clean shutdown - EOF is expected in some SSL implementations
    try {
        ssl_socket.shutdown();
    } catch (const std::system_error& e) {
        // Some SSL implementations return EOF during shutdown, which is expected
        if (e.code() != asio::error::eof) {
            Logger::Error("Shutdown error: " + std::string(e.what()));
        }
    }

    io.run();
    
}