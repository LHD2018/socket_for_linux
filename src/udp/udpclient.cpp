#include "udp/udpclient.h"


UdpClient::UdpClient(){
    m_sockfd = -1;
    m_sock_len = 0;
}

void UdpClient::initClient(const char * server_ip, const int server_port){
    udpClose();
    m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&m_serveraddr, 0, sizeof(m_serveraddr));
    m_serveraddr.sin_family = AF_INET;
    m_serveraddr.sin_addr.s_addr = inet_addr(server_ip);
    m_serveraddr.sin_port = htons(server_port);

    m_sock_len = sizeof(struct sockaddr_in);
}

bool UdpClient::udpRecv(char *buffer){
    if(m_sockfd == -1) return false;
    // 定义文件描述符集合
    fd_set tmpfd;
    // 清空集合
    FD_ZERO(&tmpfd);
    // 将socket文件描述符添加到集合
    FD_SET(m_sockfd, &tmpfd);
    // 时间值（秒和毫秒）
    struct timeval timeout;
    timeout.tv_sec = 3; 
    timeout.tv_usec = 0;

    if(select(m_sockfd + 1, &tmpfd, 0, 0, &timeout) <= 0) return false;
    

    if(recvfrom(m_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&m_serveraddr, (socklen_t *)&m_sock_len) == -1){
        return false;
    }
    return true;
}

bool UdpClient::udpSend(const char *buffer){
    if(m_sockfd == -1) return false;
    // 定义文件描述符集合
    fd_set tmpfd;
    // 清空集合
    FD_ZERO(&tmpfd);
    // 将socket文件描述符添加到集合
    FD_SET(m_sockfd, &tmpfd);
    // 时间值（秒和毫秒）
    struct timeval timeout;
    timeout.tv_sec = 3; 
    timeout.tv_usec = 0;

    if(select(m_sockfd + 1, 0, &tmpfd, 0, &timeout) <= 0) return false;
    if(sendto(m_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&m_serveraddr, m_sock_len) == -1){
        return false;
    }
    return true;
}

void UdpClient::udpClose(){
    if(m_sockfd > 0){
        close(m_sockfd);
        m_sockfd = -1;
    }
}

UdpClient::~UdpClient(){
    udpClose();
}
