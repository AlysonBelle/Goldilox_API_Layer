


#include "../inc/node.h"


socket_t    *connection_node(struct sockaddr_in *server_socket_address)
{
    socket_t    *socket_list;
    int     sockfd;
    struct sockaddr_in  socket_address;

    socket_address = *server_socket_address;
    socket_list = NULL;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return (NULL);
    }

    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;
    socket_address.sin_port = htons(4321);

    if (bind(sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        perror("bind");
        return (NULL);
    }
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        return (NULL);
    }
    socket_list = push_socket(socket_list, sockfd);
    return (socket_list);
}


void      regular_node_config(int sockfd, struct sockaddr_in socket_address)
{
    socket_t    *socket_list;
    struct sockaddr_in server_sock_address;

    socket_list = connection_node(&server_sock_address);
    socket_list = push_socket(socket_list, sockfd);

    poll_loop(socket_list, server_sock_address);
}








