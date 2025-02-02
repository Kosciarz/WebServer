#include "WebServer.hpp"

#include <boost/asio.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <string>

using namespace boost::asio::ip;

WebServer::WebServer(boost::asio::io_context& io_context)
    : m_io_context(io_context),
      m_acceptor(io_context, tcp::endpoint(tcp::v4(), 8080)),
      m_socket(io_context),
      m_request_buffer{}
{
    std::cout << "Listening on port 8080..." << '\n';
    StartAccept();
}

void WebServer::StartAccept()
{
    m_acceptor.async_accept(m_socket,
        std::bind(&WebServer::HandleAccept, this, std::placeholders::_1));
}

void WebServer::HandleAccept(const boost::system::error_code& ec)
{
    if (!ec)
    {
        std::cout << "Client connected!" << '\n';
        m_socket.wait(tcp::socket::wait_read);
        m_socket.async_read_some(boost::asio::buffer(m_request_buffer),
            std::bind(&WebServer::HandleRead, this, std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        std::cerr << "Accept error: " << ec.message() << '\n';
        m_socket.close();
    }
}

void WebServer::HandleRead(const boost::system::error_code& ec, std::size_t bytes_read)
{
    if (!ec)
    {
        std::string request(m_request_buffer.data(), bytes_read);
        std::cout << "Request: " << '\n'
                  << request << '\n';

        std::string requested_path = GetRequestedPath();
        const std::string reply = "HTTP/1.1 200 OK\r\n\r\nRequested path: " + requested_path + "\r\n";

        boost::asio::async_write(m_socket,
            boost::asio::buffer(reply.data(), reply.size()),
            std::bind(&WebServer::HandleWrite, this, std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        std::cerr << "Read error: " << ec.message() << '\n';
        m_socket.close();
    }
}

void WebServer::HandleWrite(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    if (!ec)
    {
        std::cout << "Bytes transfered: " << bytes_transferred << '\n';
        m_socket.close();
        m_io_context.stop();
    }
    else
    {
        std::cerr << "Write error: " << ec.message() << '\n';
        m_socket.close();
    }
}

std::string WebServer::GetRequestedPath()
{
    std::string path{};
    if (auto it = std::find(m_request_buffer.begin(), m_request_buffer.end(), '/'); it != m_request_buffer.end())
        for (; *it != ' '; ++it)
            path += *it;
    return path;
}