#include <winsock2.h>
#include <iostream>
#include <fstream>

#define DATA_BUFFER 1024

int main()
{
    WSADATA wsaData;
    SOCKET oldSocket, newSocket;

    struct sockaddr_in ser, cli;
    int iLen = sizeof(cli);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed to load winsock" << std::endl;
        return 1;
    }

    oldSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (oldSocket == INVALID_SOCKET)
    {
        std::cout << "socket() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    ser.sin_family = AF_INET;
    ser.sin_port = htons(6060);
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

    newSocket = accept(oldSocket, (struct sockaddr*)&cli, &iLen);

    if (newSocket == INVALID_SOCKET)
    {
        std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 从客户端接收文件内容
    std::ofstream receivedVideo("received_video.mp4", std::ios::binary);

    char buffer[DATA_BUFFER];
    int bytesRead;

    do {
        bytesRead = recv(newSocket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0)
        {
            // 将接收到的数据写入文件
            receivedVideo.write(buffer, bytesRead);
        }
        else if (bytesRead == 0)
        {
            std::cout << "Connection closed by client" << std::endl;
        }
        else
        {
            std::cout << "recv() failed: " << WSAGetLastError() << std::endl;
            break;
        }
    } while (bytesRead > 0 && strcmp(buffer, "END_OF_FILE") != 0);

    receivedVideo.close();
    closesocket(newSocket);
    closesocket(oldSocket);
    WSACleanup();

    return 0;
}
