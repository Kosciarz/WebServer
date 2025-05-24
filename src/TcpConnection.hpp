#pragma once

#include <array>
#include <filesystem>
#include <memory>
#include <string>
#include <cstddef>

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/error_code.hpp>

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using Pointer = std::shared_ptr<TcpConnection>;

    explicit TcpConnection(asio::io_context& context, const std::filesystem::path& staticFileRoot);

    static Pointer Create(asio::io_context& context, const std::filesystem::path& staticFileRoot);

    void Start();

    void CloseConnection();

    asio::ip::tcp::socket& Socket();

private:
    void HandleRead(const asio::error_code& ec, std::size_t bytesRead);

    void HandleWrite(const asio::error_code& writeError, std::size_t bytesTransferred);

    std::string GetRequestedPath() const;

    static std::optional<std::string> GetFileContents(const std::filesystem::path& path);

private:
    asio::ip::tcp::socket m_Socket;
    std::array<char, 1024> m_RequestBuffer;

    std::filesystem::path m_StaticFilesRoot;

private:
    inline static std::size_t s_UserCount = 0;
};