#pragma once

#include <iostream>
#include <array>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "TCPServer.h"

using namespace std;

// The constructor of this class instantiates a TCP client which connects to a given address on a given port to receive data
class TCPClient : public TCPServer
{
    private:
        struct sockaddr_in server;
    public:
        
        // Sets up server for client to connect to
        TCPClient(char * address, short port);
        TCPClient(sockaddr_in server);
                
        TCPClient();

        ~TCPClient();

        // Connect to the server with indicated socket
        void connectToServer(int socket);
};