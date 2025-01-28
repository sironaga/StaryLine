/* Incldue */
#include"Conection_Sys.h"
#include<stdio.h>
#include<iostream>
#include<string>
#include<WinSock2.h>
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996) 
/* ======= */
int sock;
struct sockaddr_in addr;
WSAData wsaData;

void Conect_Init()
{
	WSACleanup();
	(void)WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)はwinsockのバージョン2.0ってこと
	sock = socket(AF_INET, SOCK_DGRAM, 0);  //AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50008);// 待ち受けポート番号を50008にする
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 送信アドレスを設定
}

void SetIP(const char* IP)
{
	addr.sin_addr.S_un.S_addr = inet_addr(IP);
}

void SendData(int nData)
{
	sendto(sock, (char*)nData, sizeof(nData), 0, (struct sockaddr*)&addr, sizeof(addr));//addrに文字列送信
}
