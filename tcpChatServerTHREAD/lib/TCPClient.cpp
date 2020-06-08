#include "TCPClient.h"
#include "TCPException.h"

using namespace std;

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
        throw ConnectException(socket, server, errno);
        return;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Socket %d connected to %s:%d\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port));
    #endif
    return;
}