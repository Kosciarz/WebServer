#include <TcpConnection.hpp>

#include <boost/asio.hpp>

TcpConnection::TcpConnection(boost::asio::io_context& io_context)
    : m_socket(io_context),
      m_request{}
{
}