#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
#include <time.h>
using namespace std;

int main(int argc, char *argv[])
{
    unsigned short sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return -1;
    }

    SOCKET sclient = socket(AF_INET, SOCK_STREAM, 0);	//建立初始连接 
    if (sclient == INVALID_SOCKET)
    {
        printf("invalid socket !");
        return -1;
    }

    sockaddr_in socket_Addr;
    socket_Addr.sin_family = 2;
    clock_t start, end;
    start = clock();

    for (int i = 0; i < 1025; ++i)
    {
        socket_Addr.sin_port = htons(i);
        socket_Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (connect(sclient, (sockaddr *)&socket_Addr, sizeof(socket_Addr)) == SOCKET_ERROR)
        {
            printf("this connect error !\n");
            closesocket(sclient); //关闭连接 
            SOCKET sclient = socket(AF_INET, SOCK_STREAM, 0);		//重新建立 
            if (sclient == INVALID_SOCKET)
            {
                printf("invalid socket !");
                return -1;
            }

            sockaddr_in socket_Addr;
            socket_Addr.sin_family = 2;
        }
        else
        {
            printf("port: %d connect successfully !\n", i);
            closesocket(sclient);
            SOCKET sclient = socket(AF_INET, SOCK_STREAM, 0);
            if (sclient == INVALID_SOCKET)
            {
                printf("invalid socket !");
                return -1;
            }

            sockaddr_in socket_Addr;
            socket_Addr.sin_family = 2;
        }
    }
    end = clock();
    printf("time : %dms", end - start);
    closesocket(sclient);
    WSACleanup();
    return 0;
}

