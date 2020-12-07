#include "udp/udpserver.h"


UdpServer::UdpServer(){
    m_sockfd = -1;
    m_addr_len = 0;
}

bool UdpServer::initServer(const unsigned int port){
    m_clientaddrs.clear();
    udpClose();
    m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&m_server_addr, 0, sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(port);

    if(bind(m_sockfd, (struct sockaddr *)&m_server_addr, sizeof(m_server_addr)) != 0){
        udpClose();
        return false;
    }
    m_addr_len = sizeof(struct sockaddr_in);
    return true;
}

char *UdpServer::getClientIP(struct sockaddr_in clientaddr){
  return(inet_ntoa(clientaddr.sin_addr));
}

bool UdpServer::udpRecv(char *buffer, const int s_timeout){
    if (m_sockfd == -1) return false;
    if (s_timeout > 0){
      // 定义文件描述符集合
      fd_set tmpfd;
      // 清空集合
      FD_ZERO(&tmpfd);
      // 将socket文件描述符添加到集合
      FD_SET(m_sockfd, &tmpfd);
      // 时间值（秒和毫秒）
      struct timeval timeout;
      timeout.tv_sec = s_timeout; 
      timeout.tv_usec = 0;

      if(select(m_sockfd + 1, &tmpfd, 0, 0, &timeout) <= 0) return false;
    }

    memset(&m_clientaddr, 0, sizeof(m_clientaddr));
    if(recvfrom(m_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&m_clientaddr, (socklen_t *)&m_addr_len) == -1){
        return false;
    }
    bool isin = false;
    for(list<sockaddr_in>::iterator it = m_clientaddrs.begin();it != m_clientaddrs.end(); it++){
        
        if(((*it).sin_port == m_clientaddr.sin_port) && (getClientIP(*it) == getClientIP(m_clientaddr))){
            isin = true;
            break;
        }
    }
    if(!isin){
        m_clientaddrs.push_back(m_clientaddr);
    }
    return true;
}

bool UdpServer::udpSend(const char *buffer){
    if (m_sockfd == -1) return false;
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

    if(sendto(m_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&m_clientaddr, m_addr_len) == -1){
        return false;
    }
    return true;
}

void UdpServer::udpClose(){
    if(m_sockfd > 0){
        close(m_sockfd);
        m_sockfd = -1;
    }
}

UdpServer::~UdpServer(){
    udpClose();
}