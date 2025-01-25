#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

#include <iomanip>
#include <iostream>
#include <thread>

using json = nlohmann::json;

using namespace boost::asio;
using namespace boost::asio::ip;
 
int main()
{

    std::cout << std::setw(4) << json::meta() << std::endl;
}
