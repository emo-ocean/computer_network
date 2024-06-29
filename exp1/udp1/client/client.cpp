#include "stdafx.h"
#include<winsock2.h>
#include <stdio.h>
#define data_buffer  1024

int main() {
    WSADATA wsadata;
    SOCKET sclient;

    // ��������ַ���˿ں�
    const char* serip = "127.0.0.1";
    int Seriport = 6060;
    char ch[] = "qita";

    // ������SOCKET��ַ�ṹ����
    int ilen;
    // ����/�������ݴ�С
    int isend;
    int irecv;

    // Ҫ���͸�������������
    char send_buf[] = "hello, I am a client.";
    char recv_buf[data_buffer];

    // �������˵�SOCKET��ַ�ṹ
    struct sockaddr_in seradd;

    // ��ʼ�����ջ�����
    memset(recv_buf, 0, sizeof(recv_buf));
    // �ַ������ʼ��?
    memset(ch, 0, sizeof(ch));

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        printf("failed to load winsock\n");
        return 0;
    }

    // ������SOCKET��ַ�ṹ��ʼ��
    seradd.sin_family = AF_INET;
    seradd.sin_port = htons(Seriport);
    seradd.sin_addr.s_addr = inet_addr(serip);

    sclient = socket(AF_INET, SOCK_DGRAM, 0);
    if (sclient == INVALID_SOCKET) {
        printf("build socket failed!\n");
        return 0;
    }

    // �����Ӳ���Ҫ���е�ַ���׽��ֵİ󶨣�ֱ�ӷ������ݼ���
    ilen = sizeof(seradd);
    isend = sendto(sclient, send_buf, sizeof(send_buf), 0, (struct sockaddr*)&seradd, ilen);
    if (isend != 0 && isend != SOCKET_ERROR) {
        printf("data :%s,byte:%d\n", send_buf, isend);
    } else
        return 0;

    while (1) {
         scanf("%s", ch);
         sendto(sclient, ch, sizeof(ch), 0, (struct sockaddr*)&seradd, sizeof(seradd));

         // ���շ������ظ�������
         irecv = recvfrom(sclient, recv_buf, sizeof(recv_buf), 0, NULL, NULL);
        /*
        if (irecv != SOCKET_ERROR) {
            printf("respond:%s\n", recv_buf);
         }
         */
     }

    closesocket(sclient);
    WSACleanup();
    return 0;
}
