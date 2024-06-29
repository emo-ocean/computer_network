#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "conio.h"
#include "iostream"
#include "string.h"
#include "winsock2.h"
#include "stdlib.h"
#pragma comment(lib, "ws2_32")
using namespace std;

// #include <conio.h>
// #include <iostream>
// #include <string.h>
using namespace std;
// #pragam comment(lib, "ws2_32")
#define MAX_SIZE 4096
char CmdBuf[MAX_SIZE];
char command[MAX_SIZE];
char replyMsg[MAX_SIZE];
int nReplyCode;
bool bConnected = false;
SOCKET SocketControl;
SOCKET socketData;

bool sendCommand();

bool recvReply()
{   
    int nRecv = 0;
    memset(replyMsg, 0, MAX_SIZE);
    nRecv = recv(SocketControl, replyMsg, MAX_SIZE, 0);
    
    
    if (nRecv == SOCKET_ERROR)
    {
        cout << endl
            << "socket recv failed!" << endl;
        closesocket(SocketControl);
        return false;
    }
    if (nRecv > 4)
    {
        char* ReplyCodes = new char[3];
        memset(ReplyCodes, 0, 3);
        memcpy(ReplyCodes, replyMsg, 3);
        nReplyCode = atoi(ReplyCodes);
    }
    return true;
}
bool sendCommand()
{
    int nSend;
    nSend = send(SocketControl, command, strlen(command), 0);
    if (nSend == SOCKET_ERROR)
    {
        printf("socketControl create error:%d\n", WSAGetLastError());
        return false;
    }
    return true;
}

bool DataConnect(char* ServerIpAddr)
{
    //向 FTP 服务器发送 PASV 命令
    memset(command, 0, MAX_SIZE);
    memcpy(command, "PASV", strlen("PASV"));
    memcpy(command + strlen("PASV"), "\r\n", 2);
    if (!sendCommand())
    {
        return false;
    }
    //获得 PASV 命令的应答消息
    if (recvReply())
    {
        if (nReplyCode != 227)
        {
            printf("PASV 命令应答错误！");
            closesocket(SocketControl);
            return false;
        }
    }
    //解析 PASV 命令和应答消息
    char* part[6];
    if (strtok(replyMsg, "("))
    {
        for (int i = 0; i < 5; i++)
        {
            part[i] = strtok(NULL, ",");
            if (!part[i])
            {
                return false;
            }
        }
        part[5] = strtok(NULL, ")");
        if (!part[5])
            return false;
    }
    else
        return false;
    //获得 FTP 服务器的数据端口号
    unsigned short serverPort;
    serverPort = (unsigned short)(atoi(part[4]) << 8) + atoi(part[5]);
    socketData = socket(AF_INET, SOCK_STREAM, 0); //创建数据 SOCKET
    if (socketData = INVALID_SOCKET)
    {
        printf("data socket creat error: %d", WSAGetLastError());
        return false;
    }
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(serverPort);
    server_addr.sin_addr.S_un.S_addr = inet_addr(ServerIpAddr);
    //与 FTP 服务器之间建立数据 TCP 连接
    int nConnect = connect(socketData, (sockaddr*)&server_addr, sizeof(server_addr));
    if (nConnect == SOCKET_ERROR)
    {
        printf("create data TCP connection error : %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("please input param as the following: ftpclient ftpIPaddr\n");
        return 0;
    }
    if (bConnected == true)
    {
        printf("client has established the TCP control connection with server\n");
        closesocket(SocketControl);
        return 0;
    }
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);
    
    SocketControl = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(21);
    server_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    int nConnect = connect(SocketControl, (sockaddr*)&server_addr, sizeof(server_addr));
    if (nConnect == SOCKET_ERROR)
    {
        printf("client could not establish the FTP control connection with server\n");
        return 0;
    }


    //获取 Connect 应答消息
    if (recvReply())
    {
        if (nReplyCode == 220) //判断应答 Code
            printf("%s \n", replyMsg);
        else
        {
            printf("the reply msg is error\n");
            closesocket(SocketControl);
            return 0;
        }
    }


    //向服务器发送 USER 命令
    printf("FTP->USER:");
    memset(CmdBuf, 0, MAX_SIZE);
    //gets(CmdBuf, MAX_SIZE); //输入用户名并保存
    fgets(CmdBuf, MAX_SIZE,stdin);
    for (int i = 0; i < strlen(CmdBuf); i++) {
        if (CmdBuf[i] == '\n') {
            CmdBuf[i] = '\0';
        }
    }
    memset(command, 0, MAX_SIZE);
    memcpy(command, "USER ", strlen("USER "));
    memcpy(command + strlen("USER "), CmdBuf, strlen(CmdBuf));
    memcpy(command + strlen("USER ") + strlen(CmdBuf), "\r\n", 2);
    cout << command << "\n";
    
    if (!sendCommand())
        return 0;
    //获得 USER 命令的应答信息
    if (recvReply())
    {
        if (nReplyCode == 230 || nReplyCode == 331)
            printf("%s", replyMsg);
        else
        {
            printf("USER 命令应答错误\n");
            closesocket(SocketControl);
            return 0;
        }
    }
    if (nReplyCode == 331)
    {
        //向 FTP 服务器发送 PASV 命令
        printf("FTP > PASV:");
        memset(CmdBuf, 0, MAX_SIZE);
        for (int i = 0; i < MAX_SIZE; i++)
        {
            CmdBuf[i] = getchar(); //输入用户密码
            if (CmdBuf[i] == '\n')
            {
                CmdBuf[i] = '\0';
                break;
            }
            else
                printf(" * \r\n");
        }
        cout << command << "\n";
        memset(command, 0, MAX_SIZE);
        memcpy(command, "PASS ", strlen("PASS "));
        memcpy(command + strlen("PASS "), CmdBuf, strlen(CmdBuf));
        memcpy(command + strlen("PASS ") + strlen(CmdBuf), "\r\n", 2);
        sendCommand();
        cout << "recevice success" << endl;
        
        //获得 PASV 命令的应答信息
        if (recvReply())
        {
            if (nReplyCode == 230)
                printf("%s", replyMsg);
            else
            {
                printf("PASV 命令应答错误\n");
                closesocket(SocketControl);
                return 0;
            }
        }
    }

    //向 FTP 服务器发送 quit 命令
    printf("Success\n");
    printf("FTP->QUIT:");
    memset(command, 0, MAX_SIZE);
    memcpy(command, "QUIT", strlen("QUIT"));
    memcpy(command + strlen("QUIT"), "\r\n", 2);
    if (!sendCommand())
        return 0;
    //获得 quit 命令的应答信息
    if (recvReply())
    {
        //cout << nReplyCode << endl;
        if (nReplyCode == 221)
        {
            printf("%s", replyMsg);
            bConnected = false;
            closesocket(SocketControl);
            return 0;
        }
        else
        {
            printf("QUIT 命令应答错误\n");
            closesocket(SocketControl);
            return 0;
        }
        WSACleanup();
    }
}
