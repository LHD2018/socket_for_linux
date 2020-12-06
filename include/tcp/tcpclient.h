#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "tcp/mytcpsocket.hpp"

// socket客户端类

class TcpClient : public MyTcpSocket {
public:
  int  m_sockfd;    // 客户端的socket.
  char m_ip[21];    // 服务端的ip地址。
  int  m_port;      // 与服务端通信的端口。
  TcpClient();  // 构造函数。

  // 向服务端发起连接请求。
  // ip：服务端的ip地址。
  // port：服务端监听的端口。
  // 返回值：true-成功；false-失败。
  bool connectToServer(const char *ip, const int port);

  // 断开与服务端的连接
  void tcpClose();

  ~TcpClient();  // 析构函数自动关闭socket，释放资源。
};

#endif