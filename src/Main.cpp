#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "WebServer.hpp"

#include <iomanip>
#include <iostream>

using json = nlohmann::json;

int main()
{
    std::cout << std::setw(4) << json::meta() << '\n';
}

