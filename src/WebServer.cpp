#include "WebServer.hpp"
#include "TcpConnection.hpp"

#include <boost/asio.hpp>

#include <functional>
#include <iostream>
#include <memory>

using namespace boost::asio::ip;

WebServer::WebServer(boost::asio::io_context& io_context)
    : m_io_context(io_context),
      m_acceptor(io_context, tcp::endpoint(tcp::v4(), 8080))
{
    std::cout << "Listening on port 8080..." << '\n';
    StartAccept();
}

void WebServer::StartAccept()
{
    auto tcp_connection = std::make_shared<TcpConnection>(m_io_context);
    m_acceptor.async_accept(tcp_connection->socket(),
        std::bind(&WebServer::HandleAccept, this, tcp_connection, std::placeholders::_1));
}

void WebServer::HandleAccept(TcpConnection::pointer new_connection, const boost::system::error_code& ec)
{
    if (!ec)
        new_connection->Start();
    else
        std::cerr << "Accept error: " << ec.message() << '\n';

    StartAccept();
}
