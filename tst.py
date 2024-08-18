import socket
import sys

server_ip = "127.0.0.1"
server_port = 6973


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect((server_ip, server_port))

    while True:
        sys.stdout.write("> ")
        message = input()

        args = message.split(" ")

        resp_m = f"*{len(args)}\r\n"
        for arg in args:
            resp_m += f"${len(arg)}\r\n{arg}\r\n"

        sock.sendall(message.encode())
        rec = sock.recv(1024)
        print(rec.decode())
