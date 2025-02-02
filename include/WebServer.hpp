#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <boost/asio.hpp>

#include <iostream>
#include <array>

using namespace boost::asio::ip;

class WebServer
{
public:
    WebServer(boost::asio::io_context& io_context)
        : m_acceptor{io_context, tcp::endpoint(tcp::v4(), 8080)},
        m_socket{io_context},
        m_request{}
    {
        std::cout << "Listening on port 8080... " << '\n';
        StartAccept();
    }

private:
    std::string GetRequestedPath();

    void StartAccept();

    void ReadRequest();

    void HandleRequest();

private:
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;
    std::string m_request;
};

#endif // WEBSERVER_H