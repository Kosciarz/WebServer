﻿#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "TcpConnection.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
class WebServer
{
public:
    WebServer(boost::asio::io_context& io_context);

private:
    void StartAccept();

    void HandleAccept(TcpConnection::pointer new_connection, const boost::system::error_code& ec);

private:
    boost::asio::io_context& m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif // WEB_SERVER_H