


#include "../inc/node.h"

int        accept_node(int sockfd, struct sockaddr_in socket_address)
{
    int         accept_socket;
    socklen_t   socket_length;

    socket_length = sizeof(socket_address);
    while (true)
    {
        accept_socket = accept(sockfd, (struct sockaddr *)&socket_address,
          &socket_length);
        if (accept_socket)
        {
            printf("remote node joined\n");
            return (accept_socket);
        }
    }
    return (-1);
}


void        genesis_node_config(int sockfd, struct sockaddr_in socket_address)
{
    socket_t    *socket_list;

    socket_list = NULL;
    socket_list = push_socket(socket_list, sockfd);

    poll_loop(socket_list, socket_address);
}
