


import socket
import sys

def helloResponse(argument: int):

    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sockfd.connect(('127.0.0.1', argument))

    while True:
        to_receive = sockfd.recv(4096)
        print('received request from user ', to_receive.decode('utf-8'))

        print('responding')
        to_respond = 'I received ' + to_receive.decode('utf-8')
        print("to responsd is ", to_respond)
        sockfd.send(to_respond.encode('utf-8'))
        

helloResponse(int(sys.argv[1]))
