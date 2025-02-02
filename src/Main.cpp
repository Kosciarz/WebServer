#include "WebServer.hpp"
#include <boost/asio.hpp>

// TODO fix not working code - the connection doesn't work, 
// try creating it without the class again and see if it works
// and if it does add the class functionality step by step to see what is not working

int main()
{
    boost::asio::io_context io_context;

    WebServer server(io_context);

    io_context.run();
}