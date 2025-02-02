#include "WebServer.hpp"
#include <boost/asio.hpp>

#include <algorithm>
#include <array>
#include <string>
#include <iostream>

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
    m_request_buffer.fill(0);
    boost::asio::async_read(m_socket,
        boost::asio::buffer(m_request_buffer.data(), m_request_buffer.size()),
        [this](const boost::system::error_code& ec, std::size_t bytes_read)
        {
            if (!ec)
            {
                std::cout << "Bytes read: " << bytes_read << '\n';
                std::cout << "Request: " << std::string(m_request_buffer.data(), bytes_read) << '\n';

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
    /*std::string request_str(m_request_buffer.data(), m_request_buffer.size());
    std::string requested_path{};
    auto it = std::find(request_str.begin(), request_str.end(), '/');
    if (it != request_str.end())
    {
        for (; it != request_str.end() && *it != ' '; ++it)
        {
            requested_path += *it;
        }
    }*/

    /*const std::string reply =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: " + std::to_string(requested_path.size()) + "\r\n" +
        "Content-Type: text/plain\r\n"
        "\r\n" +
        "Requested path: " + requested_path + "\r\n";*/

    const std::string reply = "HTTP/1.1 200 OK\r\n"
                        "Content-Length: 13\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n"
                        "Hello, World!";

    boost::asio::async_write(m_socket,
        boost::asio::buffer(reply.data(), reply.size()),
        [this](const boost::system::error_code& ec, std::size_t bytes_written)
        {
            if (!ec)
            {
                std::cout << "Response sent!" << '\n';
                m_socket.close();
            }
            else
            {
                std::cerr << "Write error: " << ec.message() << '\n';
                m_socket.close();
            }
        });
}
