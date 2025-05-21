#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <array>
#include <filesystem>
#include <memory>
#include <string>

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using pointer = std::shared_ptr<TcpConnection>;

public:
    TcpConnection(boost::asio::io_context& ioContext);

    static pointer Create(boost::asio::io_context& ioContext);

    boost::asio::ip::tcp::socket& socket();

    void Start();

private:
    void HandleAccept(const boost::system::error_code& ec);

    void HandleRead(const boost::system::error_code& ec, std::size_t bytesRead);

    void HandleWrite(const boost::system::error_code& ec, std::size_t bytesTransfered);

    std::string GetRequestedPath() const;

    std::string GetFileContents(const std::filesystem::path& path);

private:
    boost::asio::ip::tcp::socket m_Socket;
    std::array<char, 1024> m_RequestBuffer;
    
private:
    inline static std::size_t s_UserCount = 0;
};