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

using namespace std;

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