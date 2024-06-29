import socket

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('127.0.0.1', 12345)
    client_socket.connect(server_address)

    print("TCP Client connected to server on port 12345.")

    N = 0
    while True:
        input("Press Enter to send message (exit to quit): ")
        message = str(N)
        client_socket.send(message.encode('utf-8'))

        response = client_socket.recv(1024).decode('utf-8')
        print(f"Received response: {response}")

        N += 1

        if message.lower() == 'exit':
            break

    client_socket.close()

if __name__ == "__main__":
    main()
