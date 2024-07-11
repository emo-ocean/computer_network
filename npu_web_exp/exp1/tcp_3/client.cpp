#include <winsock2.h>
#include <iostream>
#include <fstream>

#define DATA_BUFFER 1024

int main()
{
    WSADATA wsaData;
    SOCKET sClient;

    const char* Serip = "127.0.0.1";
    const int SeriPort = 6060;

    struct sockaddr_in ser;
    memset(&ser, 0, sizeof(ser));

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed to load winsock" << std::endl;
        return 1;
    }

    ser.sin_family = AF_INET;
    ser.sin_port = htons(SeriPort);
    ser.sin_addr.s_addr = inet_addr(Serip);

    sClient = socket(AF_INET, SOCK_STREAM, 0);
    if (sClient == INVALID_SOCKET)
    {
        std::cout << "socket() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    if (connect(sClient, (struct sockaddr*)&ser, sizeof(ser)) == INVALID_SOCKET)
    {
        std::cout << "connect() failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 读取视频文件
    std::ifstream videoFile("video.mp4", std::ios::binary);

    if (!videoFile.is_open())
    {
        std::cerr << "Failed to open video file" << std::endl;
        closesocket(sClient);
        WSACleanup();
        return 1;
    }

    char buffer[DATA_BUFFER];
    int bytesRead;

    // 发送文件内容
    while ((bytesRead = videoFile.readsome(buffer, sizeof(buffer))) > 0)
    {
        int isnd = send(sClient, buffer, bytesRead, 0);

        if (isnd == SOCKET_ERROR)
        {
            std::cout << "send() failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // 发送文件结束标志
    send(sClient, "END_OF_FILE", sizeof("END_OF_FILE"), 0);

    videoFile.close();
    closesocket(sClient);
    WSACleanup();

    return 0;
}
