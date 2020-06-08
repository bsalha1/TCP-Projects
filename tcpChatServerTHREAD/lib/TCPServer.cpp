#include "TCPServer.h"
#include "TCPException.h"

using namespace std;

TCPServer::TCPServer(char * address, unsigned short port)
{
    this->ip_address = address;
    this->port = port;

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, address, &server.sin_addr.s_addr);
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] TCP Server initialized on %s:%d\n", this->ip_address, this->port);
    #endif
}



TCPServer::~TCPServer()
{
    closeAllSockets();
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] TCP Server destroyed\n");
    #endif
}



int TCPServer::makeSocket()
{
    int optVal = 1;
    int newSocket;

    newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket == -1)
    {
        throw SocketInitException(errno);
        return -1;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Socket %d created\n", newSocket);
    #endif
    setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(int));
    serverSockets.insert(pair<int, sockaddr_in>(
        newSocket, this->server));
    return newSocket;
}



void TCPServer::bindSocket(int socket)
{
    if(bind(socket, (struct sockaddr*) &server, sizeof(server)) == -1)
    {
        throw BindException(socket, server, errno);
        return;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Bound socket %d to server %s:%d\n", socket, this->ip_address, this->port);
    #endif
}



void TCPServer::listenOnSocket(int socket, int n)
{
    if (listen(socket, n) == -1)
    {
        throw ListenException(socket, errno);
        return;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Server socket %d is listening...\n", socket);
    #endif
}



int TCPServer::acceptClient(int socket, sockaddr * client_addr, socklen_t * socket_length)
{
    int client_socket;
    client_socket = accept(socket, client_addr, socket_length);
    if(client_socket == -1)
    {
        throw AcceptException(client_socket, socket, errno);
        return client_socket;
    }

    clientSockets.insert(pair<int, sockaddr_in>(
        client_socket, *((struct sockaddr_in *) client_addr)));
    #ifdef INFO_MSG
    char address[15];
    inet_ntop(AF_INET, &(((struct sockaddr_in *)client_addr)->sin_addr), address, *socket_length);
    fprintf(stderr, "[INFO] Accepted connection from client socket %d on socket %d\n", client_socket, socket);
    fprintf(stderr, "[INFO] Address of client socket %d: %s\n", client_socket, address);
    #endif
    return client_socket;
}



ssize_t TCPServer::sendData(void * buffer, size_t size, int client_socket)
{
    ssize_t sentBytes = send(client_socket, buffer, size, 0);
    if(sentBytes == -1)
    {
        throw SendException(client_socket, errno);
        return 0;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Sent %ld bytes of buffer to socket %d\n", sentBytes, client_socket);
    #endif
    return sentBytes;
}



ssize_t TCPServer::receiveData(void * buffer, size_t size, int client_socket)
{
    ssize_t receivedBytes = recv(client_socket, buffer, size, 0);
    if(receivedBytes == -1)
    {
        throw ReceiveException(client_socket, errno);
        return 0;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Received %ld bytes of buffer from socket %d\n", receivedBytes, client_socket);
    #endif
    return receivedBytes;
}



void TCPServer::closeSocket(int socket)
{
    if(close(socket) == -1)
    {
        throw CloseException(socket, errno);
        return;
    }

    if(isServerSocket(socket))
    {
        serverSockets.erase(socket);
    }
    else if(isClientSocket(socket))
    {
        clientSockets.erase(socket);
    }
    
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] Socket %d closed\n", socket);
    #endif
}



void TCPServer::closeAllSockets()
{
    map<int, sockaddr_in>::iterator it = clientSockets.begin();
    while(it != clientSockets.end())
    {
        close(it->first);
        it++;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] All client sockets closed\n");
    #endif

    it = serverSockets.begin();
    while(it != serverSockets.end())
    {
        close(it->first);
        it++;
    }
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] All server sockets closed\n");
    #endif
}



void TCPServer::setServer(char * address, short port) noexcept
{
    this->ip_address = address;
    this->port = port;

    memset(&this->server, 0, sizeof(this->server));
    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    this->server.sin_addr.s_addr = inet_addr(address);
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] TCP Server initialized on %s:%d\n", address, port);
    #endif
}

bool TCPServer::isClientSocket(int socket) noexcept
{
    if(clientSockets.find(socket) == clientSockets.end())
    {
        return false;
    }
    return true;
}

bool TCPServer::isServerSocket(int socket) noexcept
{
    if(serverSockets.find(socket) == serverSockets.end())
    {
        return false;
    }
    return true;
}

size_t TCPServer::getNumSockets() noexcept { return this->clientSockets.size() + this->serverSockets.size(); }

map<int, sockaddr_in> TCPServer::getClientSockets() noexcept { return this->clientSockets; }

map<int, sockaddr_in> TCPServer::getServerSockets() noexcept { return this->serverSockets; }

sockaddr_in TCPServer::getServer() noexcept { return this->server; }
