#include "tcpserver.h"

TcpServer server;

void *pthServer(void *arg);

int main(int argc, char **argv){

    if(server.initServer(10027) == false){
        cout << "服务端初始化失败！！！" << endl;
        return -1;
    }
    cout << "等待客户端连接。。。。" << endl;
    while(1){
        if(server.tcpAccept() == false) continue;
        pthread_t pth_id;
        if(pthread_create(&pth_id, NULL, pthServer, (void *)(long)(server.m_connectfd)) != 0){
            cout << "线程创建失败！！！" << endl;
            continue; 
        }

    }

    return 0;
}

void *pthServer(void *arg){
    int clientfd = (long) arg;

    cout << "客户端(" << server.getClientIP(clientfd) << ")已连接(当前客户端数：" << server.m_clientaddrs.size() << ")" << endl;

    char str_buffer[1024];

    while (1){
        memset(str_buffer, 0, sizeof(str_buffer));
        if(server.tcpRecv(clientfd, str_buffer, 10) == false) break;
        cout << "接收：" << str_buffer << endl;

        strcpy(str_buffer, "ok");
        if(server.tcpSend(clientfd, str_buffer) == false) break;

    }
    cout << "客户端断开连接。。。" << endl;
    server.m_clientaddrs.erase(clientfd);
    close(clientfd);

    pthread_exit(0);
}