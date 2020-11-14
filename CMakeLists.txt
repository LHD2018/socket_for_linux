cmake_minimum_required(VERSION 2.8)
project(socket_test)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD "11")

include_directories("./include")

add_executable(client "./src/client_main.cpp" "./src/tcpclient.cpp")
add_executable(server "./src/server_main.cpp" "./src/tcpserver.cpp")

find_package(Threads REQUIRED)

target_link_libraries(client)
target_link_libraries(server Threads::Threads)
