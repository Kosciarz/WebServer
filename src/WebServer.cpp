#include "WebServer.hpp"
#include <boost/asio.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>

void WebServer::StartAccept()
{
    m_acceptor.async_accept(m_socket,
        [this](const boost::system::error_code& ec)
        {
            if (!ec)
            {
                std::cout << "Client connected!" << '\n';
                ReadRequest();
            }
            else
            {
                std::cerr << "Accept error: " << ec.message() << '\n';
                m_socket.close();
            }
            StartAccept();
        });
}

void WebServer::ReadRequest()
{
    std::array<char, 1024> request_buffer{};

    m_socket.wait(tcp::socket::wait_read);

    boost::asio::async_read(m_socket,
        boost::asio::buffer(request_buffer.data(), request_buffer.size()),
        [&](const boost::system::error_code& ec, std::size_t bytes_read)
        {
            if (!ec)
            {
                std::cout << "Bytes read: " << bytes_read << '\n';
                m_request = std::string(request_buffer.data(), bytes_read);
                std::cout << "Request: " << '\n'
                          << m_request << '\n';
                HandleRequest();
            }
            else
            {
                std::cerr << "Read error: " << ec.message() << '\n';
                m_socket.close();
            }
        });
}

void WebServer::HandleRequest()
{
    std::string requested_path = GetRequestedPath();

    const std::string reply =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: " +
        std::to_string(requested_path.size()) + "\r\n" +
        "Content-Type: text/plain\r\n"
        "\r\n" +
        "Requested path: " + requested_path + "\r\n";

    boost::asio::async_write(m_socket,
        boost::asio::buffer(reply.data(), reply.size()),
        [this](const boost::system::error_code& ec, std::size_t bytes_written)
        {
            if (!ec)
            {
                std::cout << "Bytes written: " << bytes_written << '\n';
                m_socket.close();
            }
            else
            {
                std::cerr << "Write error: " << ec.message() << '\n';
                m_socket.close();
            }
        });
}

std::string WebServer::GetRequestedPath()
{
    std::string path{};
    if (auto it = std::find(m_request.begin(), m_request.end(), '/'); it != m_request.end())
        for (; *it != ' '; ++it)
            path += *it;
    return path;
}