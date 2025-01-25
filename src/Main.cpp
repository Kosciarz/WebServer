#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include "WebServer.h"

#include <iomanip>
#include <iostream>

using json = nlohmann::json;

int main()
{
    std::cout << std::setw(4) << json::meta() << '\n';
}

