#include <iostream>
#include <memory>
#include <filesystem>

#include <asio/ip/tcp.hpp>
#include <asio/error_code.hpp>

#include "TcpConnection.hpp"
#include "WebServer.hpp"

using namespace asio::ip;
namespace fs = std::filesystem;

WebServer::WebServer(asio::io_context& ioContext)
    : m_IoContext{ioContext},
      m_Acceptor{ioContext, tcp::endpoint(tcp::v4(), 8080)},
      m_StaticFilesRoot{FindServerRoot() / "www"}
{
    std::cout << "Listening on port 8080..." << '\n';
    StartAccept();
}

void WebServer::StartAccept()
{
    auto connection = TcpConnection::Create(m_IoContext, m_StaticFilesRoot);
    m_Acceptor.async_accept(
        connection->Socket(),
        [this, connection](const asio::error_code& ec)
        {
            this->HandleAccept(connection, ec);
        }
    );
}

void WebServer::HandleAccept(const TcpConnection::Pointer& connection, const asio::error_code& ec)
{
    if (!ec)
        connection->Start();
    else
        std::cerr << "Accept error: " << ec.message() << '\n';

    StartAccept();
}

fs::path WebServer::FindServerRoot()
{
    auto path = fs::current_path();
    while (path.has_relative_path() && path.filename() != "WebServer")
    {
        if (path == path.root_path())
        {
            return {};
        }
        path = path.parent_path();
    }
    return path;
}