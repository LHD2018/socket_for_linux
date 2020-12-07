#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "common.h"

class UdpClient{
private:
    int m_sockfd;   // 客户端socket
    socklen_t m_addr_len;   // 结构体struct sockaddr_in的大小
public:
    sockaddr_in m_serveraddr;   // 服务端sockaddr

    UdpClient();

    // 初始化客户端
    void initClient(const char * server_ip, const int server_port);

    // 客户端接收
    bool udpRecv(char *buffer);

    // 客户端发送
    bool udpSend(const char *buffer);

    // 关闭客户端
    void udpClose();

    ~UdpClient();

};

#endif