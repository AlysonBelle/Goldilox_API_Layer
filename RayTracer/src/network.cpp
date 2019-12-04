# include "network.hpp"

UDPConnection::UDPConnection( ) {
    quit = false;
}

UDPConnection::~UDPConnection( ) {
    SDLNet_FreePacket(packet);
    SDLNet_Quit();
}

bool UDPConnection::Init( const std::string &ip, int32_t remotePort, int32_t localPort ) {
    std::cout << "Connecting to \n\tIP : " << ip << "\n\tPort : " << remotePort << std::endl;
    std::cout << "Local port : " << localPort << "\n\n";

    if ( !InitSDL_Net() )
        return false;

    if ( !OpenPort( localPort  ) )
        return false;

    if ( !SetIPAndPort( ip, remotePort ) )
        return false;

    if ( !CreatePacket( 512 ) )
        return false;

    return true;
}

bool UDPConnection::InitSDL_Net() {
    std::cout << "Initializing SDL_net...\n";

    if ( SDLNet_Init() == -1 )
    {
        std::cout << "\tSDLNet_Init failed : " << SDLNet_GetError() << std::endl;
        return false; 
    }

    std::cout << "\tSuccess!\n\n";
    return true;
}

bool UDPConnection::CreatePacket( int32_t packetSize ) {
    std::cout << "Creating packet with size " << packetSize << "...\n";

    // Allocate memory for the packet
    packet = SDLNet_AllocPacket( packetSize );

    if ( packet == nullptr )
    {
        std::cout << "\tSDLNet_AllocPacket failed : " << SDLNet_GetError() << std::endl;
        return false; 
    }

    // Set the destination host and port
    // We got these from calling SetIPAndPort()
    packet->address.host = serverIP.host; 
    packet->address.port = serverIP.port;

    std::cout << "\tSuccess!\n\n";
    return true;
}

bool UDPConnection::OpenPort( int32_t port ) {
    std::cout << "Opening port " << port << "...\n";

    // Sets our sovket with our local port
    ourSocket = SDLNet_UDP_Open( port );

    if ( ourSocket == nullptr )
    {
        std::cout << "\tSDLNet_UDP_Open failed : " << SDLNet_GetError() << std::endl;
        return false; 
    }

    std::cout << "\tSuccess!\n\n";
    return true;
}

bool UDPConnection::SetIPAndPort( const std::string &ip, uint16_t port ) {
    std::cout << "Setting IP ( " << ip << " ) " << "and port ( " << port << " )\n";

    // Set IP and port number with correct endianess
    if ( SDLNet_ResolveHost( &serverIP, ip.c_str(), port )  == -1 )
    {
        std::cout << "\tSDLNet_ResolveHost failed : " << SDLNet_GetError() << std::endl;
        return false; 
    }

    std::cout << "\tSuccess!\n\n";
    return true; 
}

bool UDPConnection::Send( ) {

    if (msg == prev_msg) {
        return false;
    }
    prev_msg = msg;
    memcpy(packet->data, msg.c_str(), msg.length() + 1);
    packet->len = msg.length() + 1;
    if ( SDLNet_UDP_Send(ourSocket, -1, packet) == 0 ) {
        std::cout << "SDLNet_UDP_Send failed : " << SDLNet_GetError() << "\n";
        return false; 
    } else {
        std::cout << "Sent: " << msg << "\n";
    }
    if ( msg == "quit" )
        quit = true;
    return true;
}

void UDPConnection::Recv() {
    if ( SDLNet_UDP_Recv(ourSocket, packet)) {
        bzero(recv_msg, 50);
        memcpy(recv_msg, packet->data, packet->maxlen );
        std::cout << "Data received : " << packet->data << " - " << recv_msg << "\n";
        if ( strcmp(recv_msg, "quit") == 0)
            quit = true;
    }
}

bool UDPConnection::WasQuit() {
    return quit;
}
