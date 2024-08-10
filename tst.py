import socket

server_ip = "127.0.0.1"
server_port = 6973

message = "*2\r\n$3\r\nGET\r\n$3\r\nkey\r\n"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect((server_ip, server_port))
    sock.sendall(message.encode())
