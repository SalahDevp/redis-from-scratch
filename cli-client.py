import socket
import sys

server_ip = "127.0.0.1"
server_port = 6973


def handleResponse(args):
    i = 0
    while i < len(args):
        if args[i] == "+":
            print(args[i][1:])
        elif args[i].startswith("$"):
            print(args[i + 1])

        i += 1


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect((server_ip, server_port))

    while True:
        sys.stdout.write("> ")
        message = input()

        args = message.split(" ")

        resp_m = f"*{len(args)}\r\n"
        for arg in args:
            resp_m += f"${len(arg)}\r\n{arg}\r\n"

        sock.sendall(resp_m.encode())
        rec = sock.recv(1024)
        handleResponse(rec.decode().split("\r\n"))
