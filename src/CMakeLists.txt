set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCES
    "${SOURCE_DIR}/Main.cpp"
    "${SOURCE_DIR}/WebServer.cpp"
    "${SOURCE_DIR}/TcpConnection.cpp"
)

set(HEADERS
    "${SOURCE_DIR}/WebServer.hpp"
    "${SOURCE_DIR}/TcpConnection.hpp"
)

find_package(Boost REQUIRED COMPONENTS system)

set(EXE_NAME "WebServer")
add_executable(${EXE_NAME} ${SOURCES})
target_include_directories(${EXE_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${EXE_NAME} PRIVATE Boost::system)