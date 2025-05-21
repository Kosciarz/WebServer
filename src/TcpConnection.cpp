#include "TcpConnection.hpp"
#include "WebServer.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <array>
#include <chrono>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace boost::asio::ip;
using namespace std::placeholders;

namespace fs = std::filesystem;

TcpConnection::TcpConnection(boost::asio::io_context& ioContext)
    : m_Socket{ioContext}
{
}

TcpConnection::pointer TcpConnection::Create(boost::asio::io_context& ioContext)
{
    return std::make_shared<TcpConnection>(ioContext);
}

void TcpConnection::Start()
{
    std::cout << "Client connected. ID(" << ++TcpConnection::s_UserCount << ")." << '\n';

    m_Socket.async_read_some(boost::asio::buffer(m_RequestBuffer.data(), m_RequestBuffer.size()),
        std::bind(&TcpConnection::HandleRead, shared_from_this(), _1, _2));
}

void TcpConnection::HandleRead(const boost::system::error_code& ec, std::size_t bytesRead)
{
    if (!ec)
    {
        std::cout << "Request: " << '\n'
                  << std::string(m_RequestBuffer.data(), bytesRead) << '\n';

        std::string reply{};

        try
        {
            const auto path = fs::path{"../../../www"} / GetRequestedPath();
            reply = "HTTP/1.1 200 OK\r\n\r\n" + GetFileContents(path) + "\r\n";
        }
        catch (const std::exception& e)
        {
            reply = "HTTP/1.1 400\r\n\r\n Not Found\r\n";
        }

        boost::asio::async_write(m_Socket,
            boost::asio::buffer(reply.data(), reply.size()),
            std::bind(&TcpConnection::HandleWrite, shared_from_this(), _1, _2));
    }
    else
    {
        std::cerr << "Read error: " << ec.message() << '\n';
        m_Socket.close();
        TcpConnection::s_UserCount--;
    }
}

void TcpConnection::HandleWrite(const boost::system::error_code& ec, std::size_t bytesTransfered)
{
    if (!ec)
        std::cout << "Bytes transfered: " << bytesTransfered << '\n';
    else
        std::cerr << "Write error: " << ec.message() << '\n';

    m_Socket.close();
    TcpConnection::s_UserCount--;
}

std::string TcpConnection::GetRequestedPath() const
{
    std::string path{};
    if (auto it = std::find(m_RequestBuffer.begin(), m_RequestBuffer.end(), '/'); it != m_RequestBuffer.end())
        for (; *it != ' '; ++it)
            path += *it;

    return (path == "/") ? path + "index.html" : path;
}

std::string TcpConnection::GetFileContents(const fs::path& path)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Could not open file with path: " + path.string());

    const auto fileSize = std::filesystem::file_size(path);
    std::string contents(fileSize, 0);
    file.read(contents.data(), fileSize);
    return contents;
}

boost::asio::ip::tcp::socket& TcpConnection::socket()
{
    return m_Socket;
}