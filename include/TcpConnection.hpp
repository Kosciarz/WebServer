#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost/asio.hpp>

class TcpConnection
{
public:
    TcpConnection(boost::asio::io_context& io_context);

private:
    boost::asio::ip::tcp::socket m_socket;
    std::array<char, 1024> m_request;
};

#endif //TCP_CONNECTION_H