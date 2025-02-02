#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <boost/asio.hpp>

class WebServer
{
public:
    WebServer(boost::asio::io_context& io_context);

private:
    void StartAccept();

    void ReadRequest();

    void HandleRequest();

    std::string GetRequestedPath();

private:
    boost::asio::io_context& m_io_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    std::array<char, 1024> m_request_buffer;
};

#endif // WEBSERVER_H