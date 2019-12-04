#ifndef NETWORK_H
# define NETWORK_H

# include "SDL_net.h"
# include <iostream>
# include <sstream>
# include <thread>
# include <stdio.h>
# include <string.h>

class  UDPConnection {

public:
    UDPConnection( );
    ~UDPConnection( );

    bool    Init( const std::string &ip, int32_t remotePort, int32_t localPort );
    bool    InitSDL_Net();
    bool    CreatePacket( int32_t packetSize );
    bool    OpenPort( int32_t port );
    bool    SetIPAndPort( const std::string &ip, uint16_t port ) ;
    bool    Send();
    void    Recv();
    bool    WasQuit();

    private:
	    bool        quit;
	    UDPsocket   ourSocket;
	    IPaddress   serverIP;
	    UDPpacket   *packet;
        std::string prev_msg;

    public:
        std::string msg;
        char        recv_msg[50];

};

void    send(void *udp);
void    recv(void *udp);

#endif