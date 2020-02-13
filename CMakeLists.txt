cmake_minimum_required(VERSION 3.15)
project(TlsServer)

set (CMAKE_CXX_STANDARD 11)

set (SSL_LIBS -lssl -lcrypto)

set (CMAKE_CXX_FLAGS "--std=c++11 -finput-charset=utf8 -Wall")

include_directories (src/socket/include src/exception/include)

set (SOURCE_FILES
        src/main.cpp
        src/socket/src/TcpServer.cpp
        src/socket/src/TlsServer.cpp
        src/exception/src/IoException.cpp
        src/exception/src/NetworkException.cpp)

add_executable (TlsServer ${SOURCE_FILES})

target_link_libraries (TlsServer ${SSL_LIBS})