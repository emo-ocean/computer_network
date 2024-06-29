import socket

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 12345))
    server_socket.listen(5)

    print("TCP Server is listening on port 12345...")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"Accepted connection from {client_address}")

        while True:
            data = client_socket.recv(1024)
            if not data:
                break

            message = data.decode('utf-8')
            print(f"Received from {client_address}: {message}")

            response = f"{message}已经收到"
            client_socket.send(response.encode('utf-8'))
            print(f"Sent to {client_address}: {response}")

        client_socket.close()
        print(f"Connection with {client_address} closed.")

if __name__ == "__main__":
    main()
