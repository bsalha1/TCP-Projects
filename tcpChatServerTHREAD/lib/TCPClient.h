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

TCPClient::TCPClient(char * address, short port)
{
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, address, &server.sin_addr.s_addr);
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] TCP Client initialized on %s:%d\n", address, port);
    #endif
}

TCPClient::TCPClient(sockaddr_in server)
{
    this->server = server;
}

TCPClient::TCPClient(){}

TCPClient::~TCPClient(){}

void TCPClient::connectToServer(int socket)
{
    if(connect(socket, (struct sockaddr *) &server, sizeof(server)) == -1) // if failed
    {
        throw connectException(socket, server, errno);
        return;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Socket %d connected to %s:%d\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port));
    #endif
    return;
}