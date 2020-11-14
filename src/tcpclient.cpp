#include "tcpclient.h"

TcpClient::TcpClient(){
  m_sockfd=-1;
  memset(m_ip,0,sizeof(m_ip));
  m_port=0;
  m_istimeout=false;
}

bool TcpClient::connectToServer(const char *ip, const int port){
  if (m_sockfd != -1) { 
    close(m_sockfd); 
    m_sockfd = -1; 
  }

  strcpy(m_ip,ip);
  m_port=port;

  struct hostent* h;
  struct sockaddr_in servaddr;

  if ( (m_sockfd = socket(AF_INET,SOCK_STREAM,0) ) < 0) return false;

  // 通过域名获取ip
  if ( !(h = gethostbyname(m_ip)) ){
    close(m_sockfd);  
    m_sockfd = -1; 
    return false;
  }

  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(m_port);  // 指定服务端的通讯端口
  memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

  if (connect(m_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0){
    close(m_sockfd);  
    m_sockfd = -1; 
    return false;
  }

  return true;
}


void TcpClient::tcpClose(){
  if (m_sockfd > 0) close(m_sockfd); 

  m_sockfd=-1;
  memset(m_ip,0,sizeof(m_ip));
  m_port=0;
  m_istimeout=false;
}

TcpClient::~TcpClient()
{
  tcpClose();
}


