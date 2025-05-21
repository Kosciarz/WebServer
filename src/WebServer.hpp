#pragma once

#include "TcpConnection.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

class WebServer
{
public:
    WebServer(boost::asio::io_context& ioContext);

private:
    void StartAccept();

    void HandleAccept(TcpConnection::pointer newConnection, const boost::system::error_code& ec);

private:
    boost::asio::io_context& m_IoContext;
    boost::asio::ip::tcp::acceptor m_Acceptor;
};