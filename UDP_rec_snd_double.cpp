#include <stdio.h>
#include <iostream>
#include <string>
#include <WinSock2.h> //windows
#include <cstdlib>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable:4996) //C4996エラーを無効にする

//WindowsAPIを使っているのでWindows以外では動きません

int main(int argc, char* argv[]) {
    int sock;
    WSAData wsaData;

    WSACleanup();
    WSAStartup(MAKEWORD(2, 0), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信

    //送信用
    struct sockaddr_in snd;
    snd.sin_family = AF_INET;
    snd.sin_port = htons(18001);//送信用ポート
    snd.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 送信アドレスを設定

    //受信用
    struct sockaddr_in rec;
    rec.sin_family = AF_INET;
    rec.sin_port = htons(18001);//受信用ポート
    rec.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&rec, sizeof(rec));
    
    //ノンブロッキング設定
    u_long val = 1;
    ioctlsocket(sock, FIONBIO, &val);

    double buf[1024*3];    
    //double data[] = { 1.5, 2.50, 3.001 };
    
    while (1) {
        //ランダムな数値の送信データ作成
        double rndnum = rand() % 100;
        double senddata[] = { rndnum, rndnum + 5, rndnum + 10 };

        memset(buf, 0, 1024*3);
        //データ受信
        int datacheck = recv(sock, (char*)(&buf), sizeof(buf), 0);
        if (datacheck >= 0) {
            std::cout << buf << std::endl;
            std::cout << "receive data 1は" << buf[0] << "receive data 2は" << buf[1] << "receive data 3は" << buf[2] << std::endl;
        }

        //データ送信
        sendto(sock, (const char*)(&senddata), sizeof(senddata), 0, (struct sockaddr*)&snd, sizeof(snd));
        
        Sleep(1000); //1000ms待つ
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}