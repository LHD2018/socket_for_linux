#include "tcpclient.h"

int main(int argc, char **argv){

    TcpClient client;

    if(client.connectToServer("127.0.0.1", 10027) == false){
        cout << "连接失败！！！" << endl;
        return -1;
    }
    
    char str_buffer[1024];
    for(int i = 0;i < 10;i++){
        memset(str_buffer, 0, sizeof(str_buffer));
        snprintf(str_buffer, 50, "hello %d", i);
        if(client.tcpSend(client.m_sockfd, str_buffer) == false) break;

        memset(str_buffer, 0, sizeof(str_buffer));
        if(client.tcpRecv(client.m_sockfd, str_buffer, 10) == false) break;
        cout << "接收：" << str_buffer << endl;
        sleep(1);
    }
    return 0;
}

