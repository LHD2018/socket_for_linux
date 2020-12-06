#include "udp/udpclient.h"

const char *server_ip = "127.0.0.1";

const int server_port = 10227;

int main(int argc, char **argv){
    UdpClient client;
    char buffer[1024];
    while(true){
        int lost_count = 0;
        client.initClient(server_ip, server_port);
        while(true){
            if(client.udpSend("hello") == false){
                lost_count++;
                if(lost_count < 5){
                    continue;
                }
                cout << "断开！" << endl;
                break;
            }
            memset(buffer, 0, sizeof(buffer));
            if(client.udpRecv(buffer) == false){
               lost_count++;
                if(lost_count < 5){
                    continue;
                }
                cout << "断开！" << endl;
                break;
            }
            cout << buffer << endl;
        }

    }

    return 0;
}