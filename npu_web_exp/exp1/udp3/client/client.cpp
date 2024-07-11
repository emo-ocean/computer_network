#include "stdafx.h"
#include<winsock2.h>
#include <stdio.h>
#include <fstream>

#define data_buffer  1024
#define file_buffer_length 1024

void SendFile(SOCKET clientSocket) {
    std::ifstream inputFile("video.mp4", std::ios::binary);
    if (!inputFile.is_open()) {
        printf("无法打开文件以读取\n");
        return;
    }

    char fileBuffer[file_buffer_length];
    int bytesRead;

    do {
        inputFile.read(fileBuffer, sizeof(fileBuffer));
        bytesRead = inputFile.gcount();

        if (bytesRead > 0) {
            send(clientSocket, fileBuffer, bytesRead, 0);
        }
    } while (bytesRead > 0);

    inputFile.close();
    printf("文件发送完成\n");
}

int main() {
    WSADATA wsadata;
    SOCKET sclient;

    const char* serip = "127.0.0.1";
    int Seriport = 6060;

    int ilen;
    int isend;
    char send_buf[] = "Sending video file";

    struct sockaddr_in seradd;

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("failed to load winsock\n");
        return 0;
    }

    seradd.sin_family = AF_INET;
    seradd.sin_port = htons(Seriport);
    seradd.sin_addr.s_addr = inet_addr(serip);

    sclient = socket(AF_INET, SOCK_DGRAM, 0);
    if (sclient == INVALID_SOCKET) {
        printf("build socket failed!\n");
        return 0;
    }

    ilen = sizeof(seradd);
    isend = sendto(sclient, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&seradd, ilen);
    if (isend != 0 && isend != SOCKET_ERROR) {
        printf("客户所发的数据为：%s, 字节数为：%d\n", send_buf, isend);
    } else
        return 0;

    // 发送文件
    SendFile(sclient);

    closesocket(sclient);
    WSACleanup();

    return 0;
}
