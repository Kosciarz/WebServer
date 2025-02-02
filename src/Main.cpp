#include "WebServer.hpp"
#include <boost/asio.hpp>



int main()
{
    boost::asio::io_context io_context;

    WebServer server(io_context);

    io_context.run();
}