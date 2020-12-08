#include "udp/udpclient.h"


void UdpClient::initClient(const char * server_ip, const int server_port){
    udpClose();
    m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&m_serveraddr, 0, sizeof(m_serveraddr));
    m_serveraddr.sin_family = AF_INET;
    m_serveraddr.sin_addr.s_addr = inet_addr(server_ip);
    m_serveraddr.sin_port = htons(server_port);

    m_addr_len = sizeof(struct sockaddr_in);
}

bool UdpClient::udpRecv(char *buffer, const int s_timeout){
    
    return udpRead(buffer, &m_serveraddr, s_timeout);
}

bool UdpClient::udpSend(const char *buffer){
    
    return udpWrite(buffer, &m_serveraddr);
}
