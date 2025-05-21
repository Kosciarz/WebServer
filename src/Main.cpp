#include "WebServer.hpp"

#include <iostream>

int main()
{
    try
    {
        boost::asio::io_context ioContext;
        WebServer server(ioContext);
        ioContext.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}