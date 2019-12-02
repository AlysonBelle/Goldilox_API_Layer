



#include "../inc/node.h"

struct pollfd fds[4];
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
    pollfd_list_length = 0;
    reset_pollfd_list(socket_list);
}


void    api_loop(int sockfd, socket_t *socket_list, int to_ignore)
{
    unsigned char buffer[4096];

    bzero(buffer, 4096);
    while (recv(sockfd, buffer, 4096, 0) == 0)
        ;
   
    socket_t *current;
    int counter;

    counter = 0;
    current = socket_list->next;
    while (current)
    {
        if (current->sockfd == sockfd || current->sockfd == to_ignore)
        {
            current = current->next;
            continue ;

        } 
        if (buffer[0] == 0)
            break;
        send(current->sockfd, buffer, 4096, 0);
        current = current->next;
    }
    bzero(buffer, 4096);

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
            api_loop(fds[counter].fd, socket_list, socket_to_ignore);
            break;
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

