#include <iostream>
#include <algorithm>
#include <map>
#include <iterator>
#include <exception>


#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
    Thrown when failure to send data to a socket
    @param socket socket which failed to be written to
*/
class sendException : public std::exception
{
    private:
        const int _socket;
        char * error_msg;

    public:
        sendException(const int socket, const int err_num) : _socket(socket) 
        {
            sprintf(error_msg, "Failed to send data to socket %d: %s\n", _socket, strerror(err_num));
        }

        const char * what () const throw (){ return error_msg; }

        const int getSocket(){ return _socket; }
};



/**
    Thrown when one socket fails to accept another
    @param accepted_socket socket to be accepted with acceptor
    @param acceptor_socket socket attempting to accept
*/
class acceptException : public std::exception
{
    private:
        const int _accepted_socket;
        const int _acceptor_socket;
        char error_msg[61];

    public:
        acceptException(const int accepted_socket, const int acceptor_socket, const int err_num) : _accepted_socket(accepted_socket), _acceptor_socket(acceptor_socket)
        {
            sprintf(error_msg, "Failed to accept socket %d on acceptor socket %d: %s\n", accepted_socket, acceptor_socket, strerror(err_num));
        }

        const char * what () const throw (){ return error_msg; }

        const int getAcceptedSocket(){ return _accepted_socket; }
        const int getAcceptorSocket(){ return _acceptor_socket; }
};



/**
    Thrown when socket fails to initiate listening
    @param socket socket which fails to listen
*/
class listenException : public std::exception
{
    private:
        const int _socket;
        char error_msg[42];
    
    public:
        listenException(const int socket, const int err_num) : _socket(socket)
        {
            sprintf(error_msg, "Failed to listen on socket %d: %s\n", socket, strerror(err_num));
        }
        
        const char * what () const throw (){ return error_msg; }

        const int getSocket(){ return _socket; }
};



/**
    Thrown when socket fails to lose under a close() call
    @param socket socket which failed to close
*/
class closeException : public std::exception
{
    private:
        const int _socket;
        char error_msg[42];

    public:
        closeException(const int socket, const int err_num) : _socket(socket)
        {
            sprintf(error_msg, "Failed to listen on socket %d: %s\n", socket), strerror(err_num);
        }
};




/**
    Thrown when a socket fails to be bound to a server
    @param socket socket who fails to be bound
    @param server server that socket is trying to bind to
*/
class bindException : public std::exception
{
    private:
        const int _socket;
        const sockaddr_in _server;
        char error_msg[53];
    
    public:
        bindException(const int socket, const sockaddr_in server, const int err_num) : _socket(socket), _server(server)
        {
            sprintf(error_msg, "Failed to bind socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(err_num));
        }

        const char * what () const throw (){ return error_msg; }

        const int getSocket(){ return _socket; }
        const sockaddr_in getServer(){ return _server; }
};



/**
    Thrown when socket fails to initialize
*/
class socketInitException : public std::exception
{
    private:
        char error_msg[40];

    public:
        socketInitException(const int err_num)
        {
            sprintf(error_msg, "Failed to initialize socket: %s\n", strerror(err_num));
        }

        const char * what() throw(){ return error_msg; }
};



/**
    Thrown when fails to receive data from a socket
    @param socket the socket that sent the data
*/
class receiveException : public std::exception
{
    private:
        const int _socket;
        char error_msg[50];

    public:
        receiveException(const int socket, const int err_num) : _socket(socket)
        {
            sprintf(error_msg, "Failed to receive data from socket %d: %s\n", socket, strerror(err_num));
        }

        const char * what() throw(){ return error_msg; }

        const int getSocket(){ return _socket; }
};




/**
    Thrown when connection to socket from server fails
    @param socket socket that fails to connect to server
    @param server the inet socket address structure for which the socket failed to connect to
*/
class connectException : public std::exception
{
    private:
        const int _socket;
        const sockaddr_in _server;
        char error_msg[56];
    
    public:
        connectException(const int socket, const sockaddr_in server, const int err_num) : _socket(socket), _server(server)
        {
            sprintf(error_msg, "Failed to connect socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(err_num));
        }

        const char * what() throw(){ return error_msg; }

        const int getSocket(){ return _socket; }
        const sockaddr_in getServer(){ return _server; }
};