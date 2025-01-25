#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <thread>
#include <iomanip>
#include <functional>

using json = nlohmann::json;

using namespace boost::asio;
using namespace boost::asio::ip;
 
int main()
{

    std::cout << std::setw(4) << json::meta() << std::endl;
}