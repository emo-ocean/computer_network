import socket

def send_file(file_name, server_socket):
    with open(file_name, 'rb') as file:
        while True:
            data = file.read(512)
            if not data:
                break
            server_socket.send(data)

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('127.0.0.1', 12345)
    client_socket.connect(server_address)

    print("TCP Client connected to server on port 12345.")

    file_to_send = 'large_file.txt'
    send_file(file_to_send, client_socket)

    client_socket.close()

if __name__ == "__main__":
    main()
