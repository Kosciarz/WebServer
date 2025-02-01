#include "WebServer.hpp"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <span>

using namespace boost::asio::ip;

static void HandleRequest(tcp::socket socket)
{
    try
    {
        std::array<char, 1024 * 5> buffer{};

        auto bytes_read = socket.read_some(boost::asio::buffer(buffer.data(), buffer.size()));

        std::cout << "Request: " << std::string(buffer.data(), bytes_read) << '\n';

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

int main()
{
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 80));
    std::cout << "Listening on port 80... " << '\n';

    while (true)
    {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        HandleRequest(std::move(socket));
    }
}