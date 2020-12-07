#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "common.h"

class UdpServer{
private:
    socklen_t m_addr_len;   // 结构体struct sockaddr_in的大小
    sockaddr_in m_server_addr;  // 服务端sockaddr

public:
    int m_sockfd;   // 服务端socket
    sockaddr_in m_clientaddr;   // 已连接的客户端
    list<sockaddr_in> m_clientaddrs;    // 已连接的所有客户端

    UdpServer();

    // 初始化服务端
    bool initServer(const unsigned int port);

    // 获取客户端IP
    char *getClientIP(struct sockaddr_in clientaddr);

    // 服务端接收（s_timeout 超时机制，0为无限等待）
    bool udpRecv(char *buffer, const int s_timeout=0);

    // 服务端发送
    bool udpSend(const char *buffer);

    // 关闭socket
    void udpClose();

    ~UdpServer();
};

#endif