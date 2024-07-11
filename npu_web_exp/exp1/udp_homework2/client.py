import socket

def send_file(file_name, server_socket, server_address):
    with open(file_name, 'rb') as file:
        while True:
            data = file.read(512)
            if not data:
                break
            server_socket.sendto(data, server_address)

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = ('127.0.0.1', 12345)

    print("UDP Client connected to server on port 12345.")

    file_to_send = 'large_file.txt'
    send_file(file_to_send, client_socket, server_address)

    client_socket.close()

if __name__ == "__main__":
    main()
