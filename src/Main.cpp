#include "WebServer.hpp"

#include <iostream>

int main()
{
    try
    {
        boost::asio::io_context io_context;
        WebServer server(io_context);
        io_context.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}