import socket

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind(('127.0.0.1', 12345))

    print("UDP Server is listening on port 12345...")

    while True:
        data, client_address = server_socket.recvfrom(1024)

        message = data.decode('utf-8')
        print(f"Received from {client_address}: {message}")

        response = f"{message}已经收到"
        server_socket.sendto(response.encode('utf-8'), client_address)
        print(f"Sent to {client_address}: {response}")

        if message.lower() == 'exit':
            break

    server_socket.close()

if __name__ == "__main__":
    main()
