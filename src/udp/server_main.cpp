#include "udp/udpserver.h"

// 线程函数
void *pthServer(void *arg);
// 主函数退出
void mainExit(int sig);

// 线程退出函数
void pthServerExit(void *arg);

vector<long> pth_ids;   // 所有的线程id

const int MAIN_PORT = 10227;    // 初始端口

const int END_PORT = 10327;     // 限制端口

set<int> ports;     // 已开启的端口

UdpServer main_server;      // 主服务端      

pthread_mutex_t mutex;      // 互斥量

int main(int argc, char **argv){

    pthread_mutex_init(&mutex, 0);
    // 关闭全部的信号
    for (int i=0;i<100;i++) signal(i, SIG_IGN);
    // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
    // 但请不要用 "kill -9 +进程号" 强行终止
    signal(SIGINT, mainExit);
    signal(SIGTERM, mainExit);

    if(main_server.initServer(MAIN_PORT) == false){
        cout << "服务端初始化失败！！！" << endl;
        return -1;
    }
    ports.insert(MAIN_PORT);
    cout << "等待客户端连接。。。。" << endl;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    while(true){
        if(main_server.udpRecv(buffer) == false) {
            continue;
        }
        pthread_mutex_lock(&mutex);
        int port = MAIN_PORT + 1;
        for(;port < END_PORT;port++){
            if(ports.insert(port).second){
                break;
            }
        }
        if(port == END_PORT){
            cout << "端口已用完!!!" << endl;
            continue;
        }
        cout << "开启端口：" << port << endl;

        UdpServer *tmp_server = new UdpServer();
        if((*tmp_server).initServer(port) == false){
            continue;
        }
        memcpy(&(*tmp_server).m_clientaddr, &main_server.m_clientaddr, sizeof(struct sockaddr_in));
        strcpy(buffer, "udp ok");
        
        if((*tmp_server).udpSend(buffer) == false){
            delete(tmp_server);
            continue;
        }
        delete(tmp_server);

        pthread_t pth_id;
        if(pthread_create(&pth_id, NULL, pthServer, (void *)(long)port) != 0){
            cout << "线程创建失败！！！" << endl;
            continue; 
        }
        pth_ids.push_back(pth_id);
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void *pthServer(void *arg){
    pthread_cleanup_push(pthServerExit, arg);  // 设置线程清理函数。

    pthread_detach(pthread_self());  // 分离线程。

    pthread_setcanceltype(PTHREAD_CANCEL_DISABLE, NULL);  // 设置取消方式为立即取消。

    int port = (long)arg;
    UdpServer pth_server;
    if(pth_server.initServer(port) == false){
        return 0;
    }

    char recv_buffer[1024], send_buffer[1024];
    while(true){
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(send_buffer, 0, sizeof(send_buffer));
        if(pth_server.udpRecv(recv_buffer, 50) == false){
            break;
        }
        cout << "udp接收：" << recv_buffer << endl;
        strcpy(send_buffer, "udp ok");
        if(pth_server.udpSend(send_buffer) == false){
            break;
        }
        
    }
    cout << "客户端：" << pth_server.getClientIP(pth_server.m_clientaddr) << "断开连接" << endl;
    pthread_mutex_lock(&mutex);
    for(list<sockaddr_in>::iterator it = main_server.m_clientaddrs.begin();it != main_server.m_clientaddrs.end();it++){
        if(((*it).sin_port == pth_server.m_clientaddr.sin_port) && (main_server.getClientIP(*it) == main_server.getClientIP(pth_server.m_clientaddr))){
            main_server.m_clientaddrs.erase(it);
            break;
        }
    }
    pthread_cleanup_pop(1);
    pthread_exit(0);

}

void mainExit(int sig){

    main_server.udpClose();

    // 取消全部的线程。
    for (int i=0; i<pth_ids.size(); i++) pthread_cancel(pth_ids[i]);

    exit(0);
}

void pthServerExit(void *arg){
    ports.erase((long)arg);
    // 从pth_ids中删除本线程的id。
    for (int i=0;i<pth_ids.size();i++){
        if (pth_ids[i]==pthread_self()) 
            pth_ids.erase(pth_ids.begin()+i);
        
    }
    pthread_mutex_unlock(&mutex);
}
