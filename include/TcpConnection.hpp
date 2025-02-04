#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <string>
#include <filesystem>
class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using pointer = std::shared_ptr<TcpConnection>;

    TcpConnection(boost::asio::io_context& io_context);

    boost::asio::ip::tcp::socket& socket()
    {
        return m_socket;
    }

    void Start();

private:
    void HandleAccept(const boost::system::error_code& ec);

    void HandleRead(const boost::system::error_code& ec, std::size_t bytes_read);

    void HandleWrite(const boost::system::error_code& ec, std::size_t bytes_transfered);

    std::string GetRequestedPath() const;

    std::string GetFileContents(const std::filesystem::path& path);

private:
    boost::asio::ip::tcp::socket m_socket;
    std::array<char, 1024> m_request_buf;
};

#endif // TCP_CONNECTION_H