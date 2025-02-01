#include "WebServer.hpp"
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <iostream>

int main()
{
    boost::asio::io_context io_context;
    boost::asio::io_context::work work(io_context);

    WebServer server(io_context);

    io_context.run();
}