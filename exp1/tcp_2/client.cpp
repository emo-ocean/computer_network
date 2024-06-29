#include <winsock2.h>
#include <iostream>
#include <string>

#define DATA_BUFFER 512

int main()
{
    WSADATA wsaData;
    SOCKET sClient;
    int i = 0;

    const char* Serip = "127.0.0.1";
    const int SeriPort = 5050;

    int isnd;
    char sndbuf[DATA_BUFFER];

    struct sockaddr_in ser;
    memset(sndbuf, 0, DATA_BUFFER);

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
    else
    {
        // Inside the client's while loop
while (true)
{
    std::cout << std::endl;
    i++;
    std::cout << "the " << i << " time send" << std::endl;

    std::string input;
    std::cin >> input;

    if (input == "quit")
        break;

    strncpy(sndbuf, input.c_str(), DATA_BUFFER);
    isnd = send(sClient, sndbuf, sizeof(sndbuf), 0);

    if (isnd == 0)
        break;
    else if (isnd == SOCKET_ERROR)
    {
        std::cout << "send() failed: " << WSAGetLastError() << std::endl;
        break;
    }

    std::cout << "-----client send is----" << sndbuf << std::endl;

    // Receive the message from the server
    char rcvbuf[DATA_BUFFER];
    int ircv = recv(sClient, rcvbuf, sizeof(rcvbuf), 0);

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
        // Print the received message and server IP address and port
        std::cout << "-----client received from server is--------" << rcvbuf << std::endl;
    }
}

    }

    closesocket(sClient);
    WSACleanup();

    return 0;
}
