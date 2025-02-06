#include "TcpConnection.hpp"
#include "WebServer.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <array>
#include <chrono>
#include <exception>
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

TcpConnection::TcpConnection(boost::asio::io_context& io_context)
    : m_socket(io_context)
{
}

void TcpConnection::Start()
{
    std::cout << "Client connected. ID(" << ++TcpConnection::user_count << ")." << '\n';
    // std::this_thread::sleep_for(std::chrono::seconds(20));
    m_socket.async_read_some(boost::asio::buffer(m_request_buf.data(), m_request_buf.size()),
        std::bind(&TcpConnection::HandleRead, shared_from_this(), _1, _2));
}

void TcpConnection::HandleRead(const boost::system::error_code& ec, std::size_t bytes_read)
{
    if (!ec)
    {
        std::cout << "Request: " << '\n'
                  << std::string(m_request_buf.data(), bytes_read) << '\n';

        std::string reply{};

        try
        {
            const fs::path path("../../../www" + GetRequestedPath());
            reply = "HTTP/1.1 200 OK\r\n\r\n" + GetFileContents(path) + "\r\n";
        }
        catch (const std::exception& e)
        {
            reply = "HTTP/1.1 400\r\n\r\n Not Found\r\n";
        }

        boost::asio::async_write(m_socket,
            boost::asio::buffer(reply.data(), reply.size()),
            std::bind(&TcpConnection::HandleWrite, shared_from_this(), _1, _2));
    }
    else
    {
        std::cerr << "Read error: " << ec.message() << '\n';
        m_socket.close();
        TcpConnection::user_count--;
    }
}

void TcpConnection::HandleWrite(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    if (!ec)
    {
        std::cout << "Bytes transfered: " << bytes_transferred << '\n';
        m_socket.close();
        TcpConnection::user_count--;
    }
    else
    {
        std::cerr << "Write error: " << ec.message() << '\n';
        m_socket.close();
        TcpConnection::user_count--;
    }
}

std::string TcpConnection::GetRequestedPath() const
{
    std::string path{};
    if (auto it = std::find(m_request_buf.begin(), m_request_buf.end(), '/'); it != m_request_buf.end())
        for (; *it != ' '; ++it)
            path += *it;

    return (path == "/") ? path + "index.html" : path;
}

std::string TcpConnection::GetFileContents(const fs::path& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not open file with path: " + path.string());

    std::string contents{};
    std::string file_line{};
    while (std::getline(file, file_line))
    {
        contents += file_line + "\n";
    }
    file.close();
    return contents;
}
