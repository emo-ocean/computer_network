import socket

def receive_file(file_name, client_socket):
    with open(file_name, 'wb') as file:
        while True:
            data = client_socket.recv(512)
            if not data:
                break
            file.write(data)

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 12345))
    server_socket.listen(5)

    print("TCP Server is listening on port 12345...")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"Accepted connection from {client_address}")

        receive_file('received_file.txt', client_socket)

        client_socket.close()
        print(f"Connection with {client_address} closed.")

if __name__ == "__main__":
    main()
