
#include "../inc/node.h"

int         server(char **argv)
{
    int                 sockfd;
    struct sockaddr_in  socket_address;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("setting up server\n");
    if (sockfd < 0)
    {
        perror("socket");
        return (-1);
    }
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(atoi(argv[2]));

    if (bind(sockfd, (struct sockaddr *)&socket_address,
        sizeof(socket_address)) < 0)
    {
        perror("bind");
        return (-1);
    }
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        return (-1);
    }
    printf("setup complete\n");
    genesis_node_config(sockfd, socket_address);
    return (sockfd);
}


int         client(char **argv)
{
    int                 sockfd;
    struct sockaddr_in  socket_address;

    printf("setting up client\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return (-1);
    }
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(argv[2]);
    socket_address.sin_port = htons(atoi(argv[3]));

    if (connect(sockfd, (struct sockaddr *)&socket_address,
        sizeof(socket_address)) < 0)
    {
        perror("connect");
        return (-1);
    }
    printf("setup complete\n");
    regular_node_config(sockfd, socket_address);
    return (sockfd);
}

int         determine_client_or_server(char **argv)
{
    if (strcmp("-C", argv[1]) == 0)
        return (CLIENT);
    if (strcmp("-S", argv[1]) == 0)
        return (SERVER);
    return (-1);
}

int         main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("%s [port]\n", argv[0]);
        return (1);
    }

    if (determine_client_or_server(argv) == CLIENT)
        client(argv);
    else if (determine_client_or_server(argv) == SERVER)
        server(argv);
    else
        printf("Usage : tra la la\n");
    return (0);
}
