#include <iostream>
#include <WinSock2.h>
#include<time.h>
using namespace std;
#define STATUS_FAILED 0xFFFF
unsigned long serverIPAddress;
long MaxThreadNum = 200;
long ThreadCount = 0;
long *lockThreadCount = &ThreadCount;
DWORD WINAPI ScanTcpPort(LPVOID lpParam)
{
    short TcpPort = *(short *)lpParam;
    InterlockedIncrement(lockThreadCount);
    SOCKET socketid = socket(AF_INET, SOCK_STREAM, 0);
    if (socketid == INVALID_SOCKET)
    {
        cout << "Create socketid error!\n";
        return 0;
    }
    else
    {
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(TcpPort);
        serverAddress.sin_addr.S_un.S_addr = serverIPAddress;
        connect(socketid, (sockaddr *)&serverAddress, sizeof(serverAddress));
        struct fd_set write;
        FD_ZERO(&write);
        FD_SET(socketid, &write);
        struct timeval timeout;
        timeout.tv_sec = 0.3;
        timeout.tv_usec = 0;
        if (select(0, NULL, &write, NULL, &timeout) > 0)
            cout << TcpPort <<"\n";
        closesocket(socketid);
    }
    InterlockedDecrement(lockThreadCount);
    return 0;
}
int main()
{	clock_t start,end;
    start=clock();
    WSADATA WSAData;
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        cout << "WSAStartup failed: " << GetLastError() << '\n';
        ExitProcess(STATUS_FAILED);
    }
    serverIPAddress = inet_addr("127.0.0.1");
    cout << "Open TCP Ports :" << '\n';
    for (int i = 0; i < 1024; ++i)
    {
        while (ThreadCount >= MaxThreadNum)
            Sleep(20);
        DWORD ThreadId;
        CreateThread(NULL, 0, ScanTcpPort, (LPVOID) new short(i), 0, &ThreadId);
    }
    while (ThreadCount > 0)
    {
        Sleep(100);
        WSACleanup();
    }
    end=clock();
    printf("运行时间：%d",end-start); 
}
