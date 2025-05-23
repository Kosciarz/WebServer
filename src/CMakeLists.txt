set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCES
    "${SOURCE_DIR}/Main.cpp"
    "${SOURCE_DIR}/WebServer.cpp"
    "${SOURCE_DIR}/TcpConnection.cpp"
)

set(HEADERS
    "${SOURCE_DIR}/WebServer.hpp"
    "${SOURCE_DIR}/TcpConnection.hpp"
    "${SOURCE_DIR}/Result.hpp"
)

set(EXE_NAME "WebServer")
add_executable(${EXE_NAME}
    ${SOURCES} ${HEADERS}
)
target_compile_options(${EXE_NAME} PRIVATE
    -Werror -Wall -pedantic
)
target_include_directories(${EXE_NAME} PRIVATE
    ${asio_SOURCE_DIR}/asio/include
)
target_compile_definitions(${EXE_NAME} PRIVATE
    ASIO_STANDALONE
)

if (WIN32)
    target_link_libraries(${EXE_NAME} PRIVATE
        ws2_32 mswsock
    )
endif()