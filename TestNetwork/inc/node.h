
#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define CLIENT 1
#define SERVER 2

typedef struct  socket_s {
    int sockfd;
    struct socket_s *next;
}   socket_t;

socket_t    *push_socket(socket_t *socket_list, int sockfd);
int        accept_node(int sockfd, struct sockaddr_in socket_address);
int          poll_loop(socket_t *socket_list, struct sockaddr_in socket_address);
void        genesis_node_config(int sockfd, struct sockaddr_in socket_address);
void      regular_node_config(int sockfd, struct sockaddr_in socket_address);

#endif
