#include "WebServer.hpp"
#include <boost/asio.hpp>

#include <algorithm>
#include <iostream>
#include <memory>

using namespace boost::asio::ip;

std::string GetRequestedPath(std::array<char, 1024> buffer)
{
    std::string requested_path{};
    if (auto it = std::find(buffer.begin(), buffer.end(), '/'); it != buffer.end())
        for (; *it != ' '; ++it)
            requested_path += *it;
    return requested_path;
}

void HandleRequest(std::shared_ptr<tcp::socket> socket, boost::asio::io_context& io_context)
{
    std::array<char, 1024> request_buffer{};

    socket->wait(tcp::socket::wait_read);

    boost::asio::async_read(*socket,
        boost::asio::buffer(request_buffer.data(), request_buffer.size()),
        [&](const boost::system::error_code& ec, std::size_t bytes_read)
        {
            if (!ec)
                std::cout << "Bytes read: " << bytes_read << '\n';
            else
            {
                std::cerr << "Read error: " << ec.message() << '\n';
                socket->close();
            }
        });

    std::cout << std::string(request_buffer.data(), request_buffer.size()) << '\n';

    std::string requested_path = GetRequestedPath(request_buffer);

    const std::string reply = "HTTP/1.1 200 OK\r\n\r\nRequested path: " + requested_path + "\r\n";

    boost::asio::async_write(*socket,
        boost::asio::buffer(reply.data(), reply.size()),
        [&](const boost::system::error_code& ec, std::size_t bytes_written)
        {
            if (!ec)
                std::cout << "Bytes written: " << bytes_written << '\n';
            else
            {
                std::cerr << "Write error: " << ec.message() << '\n';
                socket->close();
            }
        });

    socket->close();
    io_context.stop();
}

int main()
{
    boost::asio::io_context io_context;
    boost::asio::io_context::work work(io_context);
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    auto socket = std::make_shared<tcp::socket>(io_context);
    acceptor.async_accept(*socket,
        [&](const boost::system::error_code& ec)
        {
            if (!ec)
                HandleRequest(socket, io_context);
        });

    io_context.run();
}