#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#define buffer_length 512
#define file_buffer_length 1024

void ReceiveFile(SOCKET clientSocket) {
    std::ofstream outputFile("received_video.mp4", std::ios::binary);
    if (!outputFile.is_open()) {
        printf("锟睫凤拷锟斤拷锟侥硷拷锟斤拷写锟斤拷\n");
        return;
    }

    char fileBuffer[file_buffer_length];
    int bytesRead;

    do {
        bytesRead = recv(clientSocket, fileBuffer, sizeof(fileBuffer), 0);
        if (bytesRead > 0) {
            outputFile.write(fileBuffer, bytesRead);
        }
    } while (bytesRead > 0);

    outputFile.close();
    printf("锟侥硷拷锟斤拷锟斤拷锟斤拷锟絓n");
}

int main() {
    WSADATA wsadata;
    SOCKET sSocket;

    int ilen;
    char recv_buf[buffer_length];
    struct sockaddr_in seradd, cliadd;

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("failed to load winsocket\n");
        return 0;
    }

    sSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (sSocket == INVALID_SOCKET) {
        printf("socket():%d\n", WSAGetLastError());
        return 0;
    }

    seradd.sin_family = AF_INET;
    seradd.sin_port = htons(6060);
    seradd.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sSocket, (LPSOCKADDR)&seradd, sizeof(seradd)) == SOCKET_ERROR) {
        printf(":%d\n", WSAGetLastError());
        return 0;
    }

    ilen = sizeof(cliadd);

    memset(recv_buf, 0, sizeof(recv_buf));

    while (1) {
        recvfrom(sSocket, recv_buf, buffer_length, 0, (struct sockaddr*)&cliadd, &ilen);
        printf("task ip:[%s], port:[%d]\n", inet_ntoa(cliadd.sin_addr), ntohs(cliadd.sin_port));

        ReceiveFile(sSocket);
    }

    closesocket(sSocket);
    WSACleanup();

    return 0;
}
