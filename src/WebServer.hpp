﻿#pragma once

#include <filesystem>

#include <asio/ip/tcp.hpp>
#include <asio/error_code.hpp>

#include "TcpConnection.hpp"

class WebServer
{
public:
    explicit WebServer(asio::io_context& ioContext);

private:
    void StartAccept();

    void HandleAccept(const TcpConnection::Pointer& connection, const asio::error_code& ec);

    static std::filesystem::path FindServerRoot();

private:
    asio::io_context& m_IoContext;
    asio::ip::tcp::acceptor m_Acceptor;

    std::filesystem::path m_StaticFilesRoot;
};