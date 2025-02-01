#include "WebServer.hpp"

#include <algorithm>
#include <array>
#include <string>

void WebServer::StartAccept()
{
    m_request_buffer.fill(0);
    m_acceptor.async_accept(m_socket,
        [this](const boost::system::error_code& ec)
        {
            if (!ec)
            {
                std::cout << "Client connected!" << '\n';
                HandleRequest();
            }
            else
            {
                std::cerr << "Error: " << ec.message() << '\n';
                m_socket.close();
            }
            StartAccept();
        });
}

void WebServer::ReadRequest()
{
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
                std::cerr << "Error: " << ec.message() << '\n';
                m_socket.close();
            }
        });
}

void WebServer::HandleRequest()
{
    std::string requested_path{};
    if (auto it = std::ranges::find(m_request_buffer, '/'); it != m_request_buffer.end())
    {
        for (; *it != ' '; ++it)
        {
            requested_path += *it;
        }
    }

    const std::string reply =
        "HTTP/1.1 200 OK\r\n" +
        std::format("Content-Length: {}\r\n", requested_path.size()) +
        "Content-Type: text/plain\r\n"
        "\r\n" +
        std::format("Requested path: {}\r\n", requested_path);

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
                std::cerr << "Error: " << ec.message() << '\n';
                m_socket.close();
            }
        });
}
