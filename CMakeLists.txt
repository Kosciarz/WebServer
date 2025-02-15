﻿cmake_minimum_required(VERSION 3.20)

# Set windows version to 7 to enable websocket support
if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601)
endif()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(WebServer LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(Boost_NO_WARN_NEW_VERSIONS 1)

set(SOURCES
    "src/Main.cpp"
    "src/WebServer.cpp"
    "src/TcpConnection.cpp"
)

# add_subdirectory(external/json)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system)
# target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system nlohmann_json)