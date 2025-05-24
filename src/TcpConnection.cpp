#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <format>

#include <asio/buffer.hpp>
#include <asio/write.hpp>
#include <utility>

#include "TcpConnection.hpp"

using namespace asio::ip;
using namespace std::placeholders;
namespace fs = std::filesystem;

TcpConnection::TcpConnection(asio::io_context& context, std::filesystem::path staticFileRoot)
    : m_Socket{context}, m_RequestBuffer{}, m_StaticFilesRoot{std::move(staticFileRoot)}
{
}

TcpConnection::Pointer TcpConnection::Create(asio::io_context& context, const std::filesystem::path& staticFileRoot)
{
    return std::make_shared<TcpConnection>(context, staticFileRoot);
}

void TcpConnection::Start()
{
    std::cout << std::format("Client connected. ID: {}", ++s_UserCount) << '\n';

    m_Socket.async_read_some(
        asio::buffer(m_RequestBuffer.data(), m_RequestBuffer.size()),
        [self = shared_from_this()](const asio::error_code& ec, const std::size_t bytesRead)
        {
            self->HandleRead(ec, bytesRead);
        }
    );
}

void TcpConnection::CloseConnection()
{
    m_Socket.close();
    s_UserCount--;
}

void TcpConnection::HandleRead(const asio::error_code& readError, const std::size_t bytesRead)
{
    if (!readError)
    {
        std::cout << "Request: " << '\n'
            << std::string{m_RequestBuffer.data(), bytesRead} << '\n';

        const auto path = m_StaticFilesRoot / "index.html";
        std::cout << "Requested path: " << path.string() << '\n';

        std::string reply{};
        if (const auto contents = GetFileContents(path); contents)
        {
            reply = "HTTP/1.1 200 OK\r\n\r\n" + *contents + "\r\n";
        }
        else
        {
            reply = "HTTP/1.1 400\r\n\r\n Not Found\r\n";
        }

        asio::async_write(
            m_Socket,
            asio::buffer(reply.data(), reply.size()),
            [self = shared_from_this()](const asio::error_code& writeError, const std::size_t bytesTransferred)
            {
                self->HandleWrite(writeError, bytesTransferred);
            }
        );
    }
    else
    {
        std::cerr << "Read error: " << readError.message() << '\n';
        CloseConnection();
    }
}

void TcpConnection::HandleWrite(const asio::error_code& writeError, const std::size_t bytesTransferred)
{
    if (!writeError)
    {
        std::cout << "Bytes transferred: " << bytesTransferred << '\n';
    }
    else
    {
        std::cerr << "Write error: " << writeError.message() << '\n';
    }

    CloseConnection();
}

std::string TcpConnection::GetRequestedPath() const
{
    std::string path{};
    if (auto it = std::ranges::find(m_RequestBuffer, '/'); it != m_RequestBuffer.end())
        for (; *it != ' '; ++it)
            path += *it;

    return (path == "/") ? path + "index.html" : path;
}

std::optional<std::string> TcpConnection::GetFileContents(const fs::path& path)
{
    std::ifstream file(path);
    if (!file)
        return std::nullopt;

    const auto fileSize = std::filesystem::file_size(path);
    std::string contents(fileSize, 0);
    file.read(contents.data(), static_cast<std::streamsize>(fileSize));
    return std::move(contents);
}

tcp::socket& TcpConnection::Socket()
{
    return m_Socket;
}