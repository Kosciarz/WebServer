cmake_minimum_required(VERSION 3.20)

# Set windows version to 7 to enable websocket support
if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0601)
endif()

if (POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/external/vcpkg")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(WebServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(Boost_NO_WARN_NEW_VERSIONS 1)

add_subdirectory(src)