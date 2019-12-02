


#include "../inc/node.h"

socket_t    *new_socket(int sockfd)
{
    socket_t    *new_socket;

    new_socket = (socket_t *)malloc(sizeof(socket_t));
    new_socket->sockfd = sockfd;
    new_socket->next = NULL;
    return (new_socket);
}

socket_t    *add_socket(socket_t *socket_list, int sockfd)
{
    socket_t    *current_socket;

    current_socket = socket_list;
    while (current_socket->next)
        current_socket = current_socket->next;

    current_socket->next = (socket_t *)malloc(sizeof(socket_t));
    current_socket->next->sockfd = sockfd;
    current_socket->next->next = NULL;
    return (current_socket);
}

socket_t    *push_socket(socket_t *socket_list, int sockfd)
{
    socket_t    *current_socket;

    current_socket = socket_list;
    if (current_socket == NULL)
    {
        current_socket = new_socket(sockfd);
    }
    else
    {
        current_socket = add_socket(socket_list, sockfd);
        current_socket = socket_list;
    }
    return (current_socket);
}
