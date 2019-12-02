

import sys
import socket


def helloUserApp(argument: int):

    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sockfd.connect(('127.0.0.1', argument))

    while True:

        to_send = input('Hello user$> ')

        sockfd.send(to_send.encode('utf-8'))
        received = sockfd.recv(4096)
        print('received', received.decode('utf-8'))

helloUserApp(int(sys.argv[1]))
