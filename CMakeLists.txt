cmake_minimum_required(VERSION 3.20)
project(WebServer LANGUAGES CXX VERSION 1.0.0)

# Set windows version to 7 to enable websocket support
add_definitions(-D_WIN32_WINNT=0x0601)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-linux/include")

add_subdirectory(external/json)

find_package(Boost REQUIRED COMPONENTS system)


add_executable(${PROJECT_NAME} "src/Main.cpp" "include/WebServer.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system nlohmann_json)

