#include "TcpConnection.hpp"
#include "WebServer.hpp"

#include <boost/asio.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using namespace boost::asio::ip;

TcpConnection::TcpConnection(boost::asio::io_context& io_context)
    : m_socket(io_context),
      m_request{}
{
}

void TcpConnection::Start()
{
    std::cout << "Client connected!" << '\n';

    m_socket.wait(tcp::socket::wait_read);
    m_socket.async_read_some(boost::asio::buffer(m_request.data(), m_request.size()),
        std::bind(&TcpConnection::HandleRead, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::HandleRead(const boost::system::error_code& ec, std::size_t bytes_read)
{
    if (!ec)
    {
        std::string request_str(m_request.data(), bytes_read);
        std::cout << "Request: " << '\n'
                  << request_str << '\n';

        std::string requested_path = GetRequestedPath();
        const std::string reply = "HTTP/1.1 200 OK\r\n\r\nRequested path: " + requested_path + "\r\n";

        boost::asio::async_write(m_socket,
            boost::asio::buffer(reply.data(), reply.size()),
            std::bind(&TcpConnection::HandleWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        std::cerr << "Read error: " << ec.message() << '\n';
        m_socket.close();
    }
}

void TcpConnection::HandleWrite(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    if (!ec)
    {
        std::cout << "Bytes transfered: " << bytes_transferred << '\n';
        m_socket.close();
    }
    else
    {
        std::cerr << "Write error: " << ec.message() << '\n';
        m_socket.close();
    }
}

std::string TcpConnection::GetRequestedPath()
{
    std::string path{};
    if (auto it = std::find(m_request.begin(), m_request.end(), '/'); it != m_request.end())
        for (; *it != ' '; ++it)
            path += *it;
    return path;
}
