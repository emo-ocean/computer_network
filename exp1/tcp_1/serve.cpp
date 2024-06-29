#include <winsock2.h>
#include <iostream>
#include <string>

#define DATA_BUFFER 512

int main()
{
    int i = 0;
    WSADATA wsaData;
    SOCKET oldSocket, newSocket;

    int iLen = 0;
    int ircv = 0;
    char buf[] = "I am a server";
    char fromcli[DATA_BUFFER];

    struct sockaddr_in ser, cli;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed to load winsock" << std::endl;
        return 1;
    }

    std::cout << "server waiting" << std::endl;
    std::cout << "---------------" << std::endl;

    oldSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (oldSocket == INVALID_SOCKET)
    {
        std::cout << "socket() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    ser.sin_family = AF_INET;
    ser.sin_port = htons(5050);
    ser.sin_addr.s_addr = INADDR_ANY;

    if (bind(oldSocket, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)
    {
        std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    if (listen(oldSocket, 5) == SOCKET_ERROR)
    {
        std::cout << "listen() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    iLen = sizeof(cli);

    newSocket = accept(oldSocket, (struct sockaddr*)&cli, &iLen);

    if (newSocket == INVALID_SOCKET)
    {
        std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Print client's IP address and port
    char *clientIP = inet_ntoa(cli.sin_addr);
    int clientPort = ntohs(cli.sin_port);
    std::cout << "Connected to client IP: " << clientIP << " Port: " << clientPort << std::endl;

    while (true)
    {
        i++;
        memset(fromcli, 0, DATA_BUFFER);
        ircv = recv(newSocket, fromcli, sizeof(fromcli), 0);

        if (ircv == SOCKET_ERROR)
        {
            std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
            break;
        }
        else if (ircv == 0)
        {
            break;
        }
        else
        {
            std::cout << "-----server get is--------" << fromcli << std::endl;
        }

        if (strcmp(fromcli, "quit") == 0)
        {
            break;
        }
    }

    closesocket(newSocket);
    closesocket(oldSocket);
    WSACleanup();

    return 0;
}
