#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "WebServer.hpp"

#include <iomanip>
#include <iostream>
#include <span>
#include <algorithm>

using namespace boost::asio::ip;

int main()
{
    boost::asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
    std::cout << "Listening on port 8080... " << '\n';

    while (true)
    {
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::array<char, 1024 * 5> buffer{};
        socket.async_read_some(boost::asio::buffer(buffer.data(), buffer.size()),
                               [&](const boost::system::error_code &err, size_t bytes_transfered)
                               {
                                   if (!err)
                                       std::cout << "Bytes transfered: " << bytes_transfered << '\n';
                                   else
                                       std::cout << "Error processing request." << '\n';
                               });

        std::string requested_url{};
        for (auto it = std::ranges::find(buffer, ' ') + 1; *it != ' '; ++it)
            requested_url += *it;

        std::cout << requested_url << '\n';

        std::string response = "Hello, client!\r\n";
        boost::asio::write(socket, boost::asio::buffer(response.data(), response.size()));
    }
}