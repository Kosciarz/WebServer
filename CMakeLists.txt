cmake_minimum_required(VERSION 3.20)
project(WebServer LANGUAGES CXX VERSION 1.0.0)

# Set windows version to 7 to enable websocket support
add_definitions(-D_WIN32_WINNT=0x0601)

if (POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

if (WIN32)
    set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-windows/include")
elseif (UNIX AND NOT APPLE)
    set(CMAKE_PREFIX_PATH "${VCPKG_ROOT}/installed/x64-linux/include")
endif()

set(SOURCES
  "src/Main.cpp"
  "src/WebServer.cpp"
)

add_subdirectory(external/json)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::system nlohmann_json)

