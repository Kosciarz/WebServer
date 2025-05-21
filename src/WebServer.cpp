#include "WebServer.hpp"
#include "TcpConnection.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/system/error_code.hpp>

#include <functional>
#include <iostream>
#include <memory>

using namespace boost::asio::ip;

WebServer::WebServer(boost::asio::io_context& ioContext)
    : m_IoContext{ioContext},
      m_Acceptor{ioContext, tcp::endpoint(tcp::v4(), 8080)}
{
    std::cout << "Listening on port 8080..." << '\n';
    StartAccept();
}

void WebServer::StartAccept()
{
    auto newConnection = std::make_shared<TcpConnection>(m_IoContext);
    m_Acceptor.async_accept(newConnection->socket(),
        std::bind(&WebServer::HandleAccept, this, newConnection, std::placeholders::_1));
}

void WebServer::HandleAccept(TcpConnection::pointer newConnection, const boost::system::error_code& ec)
{
    if (!ec)
        newConnection->Start();
    else
        std::cerr << "Accept error: " << ec.message() << '\n';

    StartAccept();
}
