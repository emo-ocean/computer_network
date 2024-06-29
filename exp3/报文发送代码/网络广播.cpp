#include "WINSOCK2.H"
#include "windows.h"
#include "stdio.h"
#include <iostream>
using namespace std;
#define RECV_PROT 1000
#define SEND_PORT 2000
BOOL optReturn = TRUE;
SOCKET sock;
sockaddr_in sockAddrFrom, sockAddrTo;
DWORD CreateSocket()
{
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        printf("socket lib load error!");
        return false;
    }
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("create socket failed!\n");
        WSACleanup();
        return false;
    }
    sockaddr_in sockaddrIn;
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_addr.s_addr = INADDR_ANY;
    sockaddrIn.sin_port = htons(SEND_PORT);
    if (bind(sock, (LPSOCKADDR)&sockaddrIn, sizeof(sockaddrIn)))
    {
        closesocket(sock);
        WSACleanup();
        return false;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&optReturn, sizeof(optReturn) == SOCKET_ERROR))
    {
        closesocket(sock);
        WSACleanup();
        return false;
    }
    return true;
}
DWORD BroadDataSend(char lpBuffer[])
{
    int lengthSend = 0;
    sockAddrTo.sin_family = AF_INET;
    sockAddrTo.sin_addr.s_addr = INADDR_BROADCAST;
    sockAddrTo.sin_port = htons(RECV_PROT);
    if ((lengthSend = sendto(sock, lpBuffer, strlen(lpBuffer),
                             MSG_DONTROUTE, (struct sockaddr *)&sockAddrTo, sizeof(sockaddr))) == SOCKET_ERROR)
    {
        //发送失败
        closesocket(sock);
        WSACleanup();
        return false;
    }
    return true;
}
int main()
{

    char buffer[100];
    CreateSocket();
    printf("press any key to continue");
    getchar();
    int i;
    for (i = 0; i < 100; i++)
    {

        sprintf(buffer, "data %d", i);
        BroadDataSend(buffer);
        Sleep(50);
    }
    getchar();
    return true;
}
