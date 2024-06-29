import socket

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = ('127.0.0.1', 12345)

    print("UDP Client connected to server on port 12345.")

    N = 0
    while True:
        input("Press Enter to send message (exit to quit): ")
        message = str(N)
        client_socket.sendto(message.encode('utf-8'), server_address)

        data, server_address = client_socket.recvfrom(1024)
        response = data.decode('utf-8')
        print(f"Received response from server: {response}")

        N += 1

        if message.lower() == 'exit':
            break

    client_socket.close()

if __name__ == "__main__":
    main()
