#pragma once

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

#include "max.h"
#include "TCPException.h"

// The constructor of this class instantiates a TCP Server with methods to handle the fields of such and to perform actions across the network
class TCPServer
{
    protected:
        struct sockaddr_in server;
        char * ip_address;
        short port;
        map <int, sockaddr_in> serverSockets;
        map <int, sockaddr_in> clientSockets;

    public:
        /*
            CONSTRUCTORS
        */

        /**
         * @param address Address of server to initialize
         * @param port Port of server to host on
         */
        TCPServer(char * address, unsigned short port);

        /// Empty constructor; does nothing
        TCPServer(){}
        
        /// Closes all sockets
        ~TCPServer();


        /*
            NON-STATIC METHODS
        */

        /**
         * Makes a socket on this server's port 
         * @throw socketInitException
         * @return File descriptor of socket
         */
        int makeSocket();

        /**
         * Register a socket for server use
         * @throw bindException
         * @param socket 
         */
        void bindSocket(int socket);

        /**
         * Establishes connection with client socket
         * @throw acceptException
         * @param socket Socket to accept connection from
         * @param client_addr Address structure of client (returned by reference)
         * @param socket_length Length of socket address (returned by reference)
         * @return The file descriptor for the client socket
         */
        int acceptClient(int socket, sockaddr * client_addr, socklen_t * socket_length);

        /**
         * Closes a socket for reading and writing
         * @throw closeException
         * @param socket Socket to close
         */
        void closeSocket(int socket);

        void closeAllSockets();
        


        /*
            STATIC METHODS
        */

        /**
         * Set socket to listen for n requests
         * @throw listenException
         * @param socket Socket to listen
         * @param n Number of requests allowed simultaneously
         */
        static void listenOnSocket(int socket, int n);

        /**
         * Sends a given buffer of given size to a client socket
         * @throw sendException
         * @param buffer Data to send
         * @param size Size of buffer (bytes)
         * @param client_socket Client socket to send data to
         * @return Number of bytes sent
         */
        static ssize_t sendData(void * buffer, size_t size, int client_socket);

        /**
         * Receive buffer being transmitted from client socket
         * @throw receiveException
         * @param buffer Pointer to store the data in
         * @param size Size of buffer (bytes)
         * @param client_socket Client socket who is transmitting the data
         * @return Number of bytes received
         */
        static ssize_t receiveData(void * buffer, size_t size, int client_socket);



        /*
            BOOLEANS
        */
        bool isClientSocket(int socket) noexcept;

        bool isServerSocket(int socket) noexcept;



        /*
            SETTERS
        */

        /// Set the inet address for the server
        void setServer(char * address, short port) noexcept;



        /*
            GETTERS
        */

        /// Get map of server sockets
        map<int, sockaddr_in> getServerSockets() noexcept;

        /// Get map of client sockets
        map<int, sockaddr_in> getClientSockets() noexcept;

        /// Get the inet server structure
        sockaddr_in getServer() noexcept;

        /// Get the number of sockets active on the server
        size_t getNumSockets() noexcept;
};



TCPServer::~TCPServer()
{
    closeAllSockets();
    #ifdef INFO_MSG
    fprintf(stderr, "[INFO] TCP Server destroyed\n");
    #endif
}



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
        throw bindException(socket, server, errno);
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
        throw listenException(socket, errno);
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
        throw acceptException(client_socket, socket, errno);
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
        throw sendException(client_socket, errno);
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
        throw receiveException(client_socket, errno);
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
        throw closeException(socket, errno);
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
