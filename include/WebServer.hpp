#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <boost/asio.hpp>

class WebServer
{
public:
    WebServer(boost::asio::io_context& io_context);

private:
    void StartAccept();

    void HandleAccept(const boost::system::error_code& ec);

    void HandleRead(const boost::system::error_code& ec, std::size_t bytes_read);

    void HandleWrite(const boost::system::error_code& ec, std::size_t bytes_transferred);

    std::string GetRequestedPath();

private:
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    std::array<char, 1024> m_request_buffer;
};

#endif // WEBSERVER_H