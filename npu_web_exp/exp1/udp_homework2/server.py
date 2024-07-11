import socket

def receive_file(file_name, server_socket, client_address):
    with open(file_name, 'wb') as file:
        while True:
            data, client_address = server_socket.recvfrom(512)
            if not data:
                break
            file.write(data)

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind(('127.0.0.1', 12345))

    print("UDP Server is listening on port 12345...")

    while True:
        data, client_address = server_socket.recvfrom(512)
        print(f"Received connection from {client_address}")

        receive_file('received_file.txt', server_socket, client_address)

        print(f"File received from {client_address}")

if __name__ == "__main__":
    main()
