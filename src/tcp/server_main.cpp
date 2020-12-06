#include "tcp/tcpserver.h"

// 线程函数
void *pthServer(void *arg);
// 信号2和信号15的处理函数
void mainExit(int sig);
// 线程清理函数
void pthServerExit(void *arg);

// 子线程id集合
vector<long> pth_ids;
// 服务端
TcpServer server;

int main(int argc, char **argv){

    // 关闭全部的信号
    for (int i=0;i<100;i++) signal(i, SIG_IGN);
    // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
    // 但请不要用 "kill -9 +进程号" 强行终止
    signal(SIGINT, mainExit); signal(SIGTERM, mainExit);

    if(server.initServer(10127) == false){
        cout << "服务端初始化失败！！！" << endl;
        return -1;
    }
    cout << "等待客户端连接。。。。" << endl;
    while(true){
        if(server.tcpAccept() == false) continue;
        pthread_t pth_id;
        if(pthread_create(&pth_id, NULL, pthServer, (void *)(long)(server.m_connectfd)) != 0){
            cout << "线程创建失败！！！" << endl;
            continue; 
        }
        pth_ids.push_back(pth_id);
    }

    return 0;
}

void *pthServer(void *arg){

    pthread_cleanup_push(pthServerExit, arg);  // 设置线程清理函数。

    pthread_detach(pthread_self());  // 分离线程。

    pthread_setcanceltype(PTHREAD_CANCEL_DISABLE, NULL);  // 设置取消方式为立即取消。

    int clientfd = (long) arg;

    cout << "客户端(" << server.getClientIP(clientfd) << ")已连接(当前客户端数：" << server.m_clientaddrs.size() << ")" << endl;

    char recv_buffer[1024], send_buffer[1024];

    while (true){
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(send_buffer, 0, sizeof(send_buffer));

        if(server.tcpRecv(clientfd, recv_buffer, 50) == false) break;
        cout << "tcp 接收：" << recv_buffer << endl;

        sprintf(send_buffer, "tcp ok");
        if(server.tcpSend(clientfd, send_buffer) == false) break;

    }
    cout << "客户端断开连接。。。" << endl;
    pthread_cleanup_pop(1);
    pthread_exit(0);
}

// 信号2和15的处理函数。
void mainExit(int sig){

    // 关闭监听的socket。
    server.closeListen();

    // 取消全部的线程。
    for (int i=0; i<pth_ids.size(); i++) pthread_cancel(pth_ids[i]);

    exit(0);
}

// 线程清理函数
void pthServerExit(void *arg){
    // 关闭与客户端的socket。
    close((int)(long)arg);
    server.m_clientaddrs.erase((int)(long)arg);

    // 从pth_ids中删除本线程的id。
    for (int i=0;i<pth_ids.size();i++){
        if (pth_ids[i]==pthread_self()) 
            pth_ids.erase(pth_ids.begin()+i);
        
    }

}