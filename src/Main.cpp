#include "WebServer.hpp"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <span>

using namespace boost::asio::ip;

static std::string GetRequestedPath(std::span<char> request_buffer)
{
    std::string requested_path{};
    for (auto it = std::ranges::find(request_buffer, '/'); *it != ' '; ++it)
        requested_path += *it;
    return requested_path;
}

static void HandleRequest(tcp::socket socket)
{
    try
    {
        std::array<char, 1024 * 5> request_buffer{};

        socket.wait(tcp::socket::wait_read);

        socket.async_read_some(boost::asio::buffer(request_buffer.data(), request_buffer.size()),
            [&](const boost::system::error_code& ec, std::size_t bytes_read)
            {
                if (!ec)
                    std::cout << "Bytes read: " << bytes_read << '\n';
                else
                    std::cerr << "Error: " << ec.message() << '\n';
            }
        );

        std::cout << "Request: " << std::string(request_buffer.data(), request_buffer.size()) << '\n';

        std::string requested_path = GetRequestedPath(request_buffer);
        
        const std::string reply = "HTTP/1.1 200 OK\r\n\r\nRequested path: " + requested_path + "\r\n";

        socket.async_write_some(boost::asio::buffer(reply.data(), reply.size()),
            [&](const boost::system::error_code& ec, std::size_t bytes_written)
            {
                if (!ec)
                    std::cout << "Bytes written: " << bytes_written << '\n';
                else
                    std::cerr << "Error: " << ec.message() << '\n';
            }    
        );

        socket.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        std::cout << "Listening on port 80... " << '\n';

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            HandleRequest(std::move(socket));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}