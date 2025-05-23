#pragma once

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/error_code.hpp>

#include "TcpConnection.hpp"

class WebServer
{
public:
    explicit WebServer(asio::io_context& ioContext);

private:
    void StartAccept();

    void HandleAccept(const TcpConnection::pointer& newConnection, const asio::error_code& ec);

private:
    asio::io_context& m_IoContext;
    asio::ip::tcp::acceptor m_Acceptor;
};