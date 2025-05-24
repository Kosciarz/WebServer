cmake_minimum_required(VERSION 3.20)

# Set windows version to 7 to enable websocket support
if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif ()

if (POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif ()

project(WebServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-34-2
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(asio)

add_subdirectory(src)