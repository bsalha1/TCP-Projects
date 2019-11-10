#include <iostream>
#include <algorithm>
#include <map>
#include <iterator>
#include <exception>

#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcpEnum.h"
#include "tcpException.h"

// The constructor of this class instantiates a TCP Server with methods to handle the fields of such and to perform actions across the network
class TCPServer
{
protected:
    struct sockaddr_in server;
    map <int, sockaddr_in> serverSockets;
    map <int, sockaddr_in> clientSockets;

public:
    /*
        CONSTRUCTORS
    */

    // Constructor: initializes a server of which the server will be connected to
    TCPServer(char * address, short port);

    // Empty constructor
    TCPServer(){}

    // Deconstructor: closes all sockets
    ~TCPServer();


    /*
        NON-STATIC METHODS
    */

    // Make socket on the server, returns file descriptor of socket. Socket of value -1 failed to be created
    int makeSocket();

    // Register a socket for server use. -1 for errors, 0 for success
    void bindSocket(int socket);

    // Accepts client connection on specified server socket, returns the client socket file descriptor. -1 for errors
    int acceptClient(int socket, sockaddr * clientAddr, socklen_t * socketLength);

    // Close specified socket and remove it from the list it's in 
    void closeSocket(int socket);
    


    /*
        STATIC METHODS
    */

    // Listen for requests on a socket - will prevent further requests once n requests are being processed.
    static void listenOnSocket(int socket, int n);

    // Sends a buffer of specified size to a client socket. Returns number of bytes sent or -1 for error
    static ssize_t sendData(void * buffer, int size, int clientSocket);

    // Receive buffer from specified socket. Returns number of bytes received or -1 for error
    static ssize_t receiveData(void * data, size_t size, int clientSocket);



    /*
        BOOLEANS
    */
    bool isClientSocket(int socket) noexcept;

    bool isServerSocket(int socket) noexcept;



    /*
        SETTERS
    */

    // Set the inet address for the server
    void setServer(char * address, short port) noexcept;



    /*
        GETTERS
    */

    // Get map of server sockets
    map<int, sockaddr_in> getServerSockets() noexcept;

    // Get map of client sockets
    map<int, sockaddr_in> getClientSockets() noexcept;

    // Get the inet server structure
    sockaddr_in getServer() noexcept;

    // Get the number of sockets active on the server
    size_t getNumSockets() noexcept;
};



TCPServer::~TCPServer()
{
}



TCPServer::TCPServer(char * address, short port)
{
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, address, &server.sin_addr.s_addr);
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] TCP Server initialized on %s:%d\n", address, port);
    #endif
}



int TCPServer::makeSocket()
{
    int i;
    int optVal = 1;
    int newSocket;

    newSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (newSocket == -1)
    {
        throw socketInitException(errno);
        return -1;
    }
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Socket %d created\n", newSocket);
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
        throw bindException(socket, server, errno);
        return;
    }
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Bound socket %d to server\n", socket);
    #endif
}



void TCPServer::listenOnSocket(int socket, int n)
{
    if (listen(socket, n) == -1)
    {
        throw listenException(socket, errno);
        return;
    }
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Server socket %d is listening...\n", socket);
    #endif
}



int TCPServer::acceptClient(int socket, sockaddr * clientAddr, socklen_t * socketLength)
{
    int clientSocket;
    clientSocket = accept(socket, clientAddr, socketLength);
    if(clientSocket == -1)
    {
        throw acceptException(clientSocket, socket, errno);
        return clientSocket;
    }

    clientSockets.insert(pair<int, sockaddr_in>(
        clientSocket, *((struct sockaddr_in *) clientAddr)));
    #ifdef ERR_MSG
    char address[256];
    inet_ntop(AF_INET, &(((struct sockaddr_in *)clientAddr)->sin_addr), address, *socketLength);
    fprintf(stderr, "[OK] Accepted connection from client socket %d on socket %d\n", clientSocket, socket);
    fprintf(stderr, "[INFO] Address of client socket %d: %s\n", clientSocket, address);
    #endif
    return clientSocket;
}



ssize_t TCPServer::sendData(void * buffer, int size, int clientSocket)
{
    ssize_t sentBytes = send(clientSocket, buffer, size, 0);
    if(sentBytes == -1)
    {
        throw sendException(clientSocket, errno);
        return 0;
    }
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Sent data to socket %d\n", clientSocket);
    #endif
    return sentBytes;
}



ssize_t TCPServer::receiveData(void * data, size_t size, int clientSocket)
{
    ssize_t receivedBytes = recv(clientSocket, data, size, 0);
    if(receivedBytes == -1)
    {
        throw receiveException(clientSocket, errno);
        return 0;
    }
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Received data from socket %d\n", clientSocket);
    #endif
    return receivedBytes;
}



void TCPServer::closeSocket(int socket)
{
    if(close(socket) == -1)
    {
        throw closeException(socket, errno);
        #ifdef ERR_MSG
        fprintf(stderr, "[FAIL] Socket %d failed to close: %s\n", socket, strerror(errno));
        #endif
        return;
    }
    if(isServerSocket(socket))
    {
        serverSockets.erase(socket);
    }
    else
    {
        clientSockets.erase(socket);
    }
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Socket %d closed\n", socket);
    #endif
}



void TCPServer::setServer(char * address, short port) noexcept
{
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(address);
    fprintf(stderr, "[OK] TCP Server initialized on %s:%d\n", address, port);
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

size_t TCPServer::getNumSockets() noexcept { return clientSockets.size() + serverSockets.size(); }

map<int, sockaddr_in> TCPServer::getClientSockets() noexcept { return this->clientSockets; }

map<int, sockaddr_in> TCPServer::getServerSockets() noexcept { return this->serverSockets; }

sockaddr_in TCPServer::getServer() noexcept { return this->server; }
