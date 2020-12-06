cmake_minimum_required(VERSION 2.8)
project(socket_for_linux)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD "11")

include_directories("./include")

set(TCP_SRC ${CMAKE_SOURCE_DIR}/src/tcp)
set(UDP_SRC ${CMAKE_SOURCE_DIR}/src/udp)



add_executable(tcp_client "${TCP_SRC}/client_main.cpp" "${TCP_SRC}/tcpclient.cpp")
add_executable(tcp_server "${TCP_SRC}/server_main.cpp" "${TCP_SRC}/tcpserver.cpp")

add_executable(udp_client "${UDP_SRC}/client_main.cpp" "${UDP_SRC}/udpclient.cpp")
add_executable(udp_server "${UDP_SRC}/server_main.cpp" "${UDP_SRC}/udpserver.cpp")


find_package(Threads REQUIRED)

target_link_libraries(tcp_server Threads::Threads)
target_link_libraries(udp_server Threads::Threads)
