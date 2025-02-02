#include "WebServer.hpp"
#include <boost/asio.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace boost::asio::ip;

WebServer::WebServer(boost::asio::io_context& io_context)
    : m_io_context(io_context),
      m_acceptor{io_context, tcp::endpoint(tcp::v4(), 8080)},
      m_socket{io_context},
      m_request_buffer{}
{
    std::cout << "Listening on port 8080... " << '\n';
    StartAccept();
}

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
    m_socket.wait(tcp::socket::wait_read);

    m_socket.async_read_some(boost::asio::buffer(m_request_buffer),
        [&](const boost::system::error_code& ec, std::size_t bytes_read)
        {
            if (!ec)
            {
                std::cout << "Bytes read: " << bytes_read << '\n';
                std::cout << "Request: " << '\n'
                          << std::string(m_request_buffer.data(), bytes_read) << '\n';
                // HandleRequest();
            }
            else
            {
                std::cerr << "Read error: " << ec.message() << '\n';
                m_socket.close();
            }
        });
}

// void WebServer::HandleRequest()
// {
//     const std::string requested_path = GetRequestedPath();
//     const std::string reply = "HTTP/1.1 200 OK\r\nRequested path: " + requested_path + "\r\n";

//     m_io_context.stop();
    // boost::asio::async_write(m_socket,
    //     boost::asio::buffer(reply),
    //     [this](const boost::system::error_code& ec, std::size_t bytes_transferred)
    //     {
    //         if (!ec)
    //         {
    //             std::cout << "Bytes written: " << bytes_transferred << '\n';
    //             m_socket.close();
    //         }
    //         else
    //         {
    //             std::cerr << "Write error: " << ec.message() << '\n';
    //             m_socket.close();
    //         }
    //     });


std::string WebServer::GetRequestedPath()
{
    std::string path{};
    if (auto it = std::find(m_request_buffer.begin(), m_request_buffer.end(), '/'); it != m_request_buffer.end())
        for (; *it != ' '; ++it)
            path += *it;
    return path;
}