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
    : m_socket(io_context)
{
}

void TcpConnection::Start()
{
    std::cout << "Client connected!" << '\n';
    m_socket.async_read_some(boost::asio::buffer(m_request_buf.data(), m_request_buf.size()),
        std::bind(&TcpConnection::HandleRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::HandleRead(const boost::system::error_code& ec, std::size_t bytes_read)
{
    if (!ec)
    {
        std::cout << "Request: " << '\n'
                  << std::string(m_request_buf.data(), bytes_read) << '\n';

        const std::string reply = "HTTP/1.1 200 OK\r\n\r\nRequested path: " + GetRequestedPath() + "\r\n";

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

std::string TcpConnection::GetRequestedPath() const
{
    std::string path{};
    if (auto it = std::find(m_request_buf.begin(), m_request_buf.end(), '/'); it != m_request_buf.end())
        for (; *it != ' '; ++it)
            path += *it;
    return path;
}
