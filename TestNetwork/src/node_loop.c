



#include "../inc/node.h"

struct pollfd fds[3];
int pollfd_list_length = 0;

void    push_pollfd(int sockfd)
{
    if (pollfd_list_length == 4)
        return ;

    struct pollfd   fdlist_object;

    fdlist_object.fd = sockfd;
    fdlist_object.events = POLLIN;
    fdlist_object.revents = 0;
    fds[pollfd_list_length] = fdlist_object;
    pollfd_list_length++;
}

void    reset_pollfd_list(socket_t *socket_list)
{
    socket_t    *current_socket;

    current_socket = socket_list;
    while (current_socket)
    {
        push_pollfd(current_socket->sockfd);
        current_socket = current_socket->next;
    }
}

void    init_pollfd_list(socket_t *socket_list)
{
    memset(fds, 0, sizeof(fds));
    reset_pollfd_list(socket_list);
}


void    api_loop(int sockfd, int api_socket)
{
    unsigned char buffer[4096];

    bzero(buffer, 4096);
    recv(sockfd, buffer, 4096, 0);
    send(api_socket, buffer, 4096, 0);

    bzero(buffer, 4096);
    recv(api_socket, buffer, 4096, 0);
    send(sockfd, buffer, 4096, 0);

}

void    handle_api_request_response(socket_t *socket_list, int socket_to_ignore)
{
    int counter;
    unsigned char buffer[4096];

    counter = 1;
    bzero(buffer, 4096);
    while (counter < pollfd_list_length)
    {
        if (fds[counter].revents)
        {
            api_loop(fds[counter].fd, fds[2].fd);
            break ;
        }
        counter++;
    }
}

int     poll_loop(socket_t *socket_list, struct sockaddr_in socket_address)
{
    int trigger;
    int newsocket;

    while (true)
    {
        init_pollfd_list(socket_list);
        trigger = poll(fds, pollfd_list_length, 100000);        
    
        if (trigger)
        {
           if (fds[0].revents & POLLIN)
           {
               newsocket = accept_node(fds[0].fd, socket_address);
               socket_list = push_socket(socket_list, newsocket);
           }
           else
           {
                handle_api_request_response(socket_list, fds[0].fd);
           }
        }
    }
}

