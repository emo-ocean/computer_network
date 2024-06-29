#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#define buffer_length 512

int main() {
    WSADATA wsadata;
    SOCKET sSocket;

    int ilen;
    int irecv;
    char recv_buf[buffer_length];
    // 服务器和客户SOCKET地址结构
    struct sockaddr_in seradd, cliadd;

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("failed to load winsocket\n");
        return 0;
    }

    sSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (sSocket == INVALID_SOCKET) {
        printf("socket() failed:%d\n", WSAGetLastError());
        return 0;
    }

    // 初始化服务器SOCKET地址结构
    seradd.sin_family = AF_INET;
    seradd.sin_port = htons(6060);
    seradd.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sSocket, (LPSOCKADDR)&seradd, sizeof(seradd)) == SOCKET_ERROR) {
        printf("adw:%d\n", WSAGetLastError());
        return 0;
    }

    ilen = sizeof(cliadd);

    // 初始化接收缓冲区
    memset(recv_buf, 0, sizeof(recv_buf));

    while (1) {
        // 在此函数中 cli 为传出参数
        irecv = recvfrom(sSocket, recv_buf, buffer_length, 0, (struct sockaddr*)&cliadd, &ilen);
        if (irecv == SOCKET_ERROR) {
            printf("send wrong:%d\n", WSAGetLastError());
            return 0;
        } else if (irecv == 0)
            break;
        else {
            printf("\n%s--", recv_buf);
            printf("server get: ip:[%s], port:[%d]\n", inet_ntoa(cliadd.sin_addr), ntohs(cliadd.sin_port));

            // 将客户端发送的数据再发送给客户端
            sendto(sSocket, recv_buf, irecv, 0, (struct sockaddr*)&cliadd, ilen);
        }
    }

    closesocket(sSocket);
    WSACleanup();

    return 0;
}
