#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <iomanip>
#include <iostream>
#include <thread>

using json = nlohmann::json;

int main()
{
    boost::asio::io_context io_context;

    std::cout << std::setw(4) << json::meta() << std::endl;
}
