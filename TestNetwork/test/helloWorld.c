#include <unistd.h>
#include <string.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>



int    helloSocket(char **argv)
{
    int port_number;
    int sockfd;
    struct sockaddr_in  socket_address;


    port_number = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return (-1);
    }
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    socket_address.sin_port = htons(port_number);

    if (connect(sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        perror("connect");
        return (-1);
    }
    return (sockfd);
}



int     main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage : %s\n [port] - Automatically connecting to 127.0.0.1\n", argv[0]);
        return (-1);
    }
    int sockfd;
    // setup the socket
    sockfd = helloSocket(argv);

    unsigned char buffer[4096];
    while (true)
    {
        // this is where you send your stupid string.
        bzero(buffer, 4096); // zero out the buffer to get rid of garbage.
        write(1, "Hello User$>", 12);
        read(0, buffer, 4096); // Type stuff in from the terminal.
        if (buffer[0] == '\n') // if you just hit ENTER it'll continue
            continue ;
        printf("sending message\n");
        send(sockfd, buffer, 4096, 0); // send your string to the other side
        bzero(buffer, 4096);
        recv(sockfd, buffer, 4096, 0); // receive it's response.
        printf("The server said : %s\n", buffer);

    }
    return (0);
}
