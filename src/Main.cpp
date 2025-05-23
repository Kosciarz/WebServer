#include <iostream>
#include <exception>

#include <asio/io_context.hpp>

#include "WebServer.hpp"

int main()
{
    try
    {
        asio::io_context ioContext;
        WebServer server{ioContext};
        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}