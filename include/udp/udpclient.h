#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "udp/myudpsocket.hpp"

class UdpClient : public MyUdpSocket{
public:
    // 初始化客户端
    void initClient(const char * server_ip, const int server_port);

    // 客户端接收
    bool udpRecv(char *buffer,int buff_len, const int s_timeout);

    // 客户端发送
    bool udpSend(const char *buffer);

};

#endif