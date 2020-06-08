#pragma once

#include <iostream>
#include <algorithm>
#include <map>
#include <iterator>
#include <exception>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

/**
    Thrown when failure to send data to a socket
    @param socket socket which failed to be written to
*/
class SendException : public std::exception
{
    private:
        const int _socket;
        char * error_msg;

    public:
        SendException(const int socket, const int err_num);
        const char * what () const throw ();
        const int getSocket();
};



/**
    Thrown when one socket fails to accept another
    @param accepted_socket socket to be accepted with acceptor
    @param acceptor_socket socket attempting to accept
*/
class AcceptException : public std::exception
{
    private:
        const int _accepted_socket;
        const int _acceptor_socket;
        char error_msg[61];

    public:
        AcceptException(const int accepted_socket, const int acceptor_socket, const int err_num);
        const char * what () const throw ();
        const int getAcceptedSocket();
        const int getAcceptorSocket();
};



/**
    Thrown when socket fails to initiate listening
    @param socket socket which fails to listen
*/
class ListenException : public std::exception
{
    private:
        const int _socket;
        char error_msg[42];
    
    public:
        ListenException(const int socket, const int err_num);
        const char * what () const throw ();
        const int getSocket();
};



/**
    Thrown when socket fails to lose under a close() call
    @param socket socket which failed to close
*/
class CloseException : public std::exception
{
    private:
        const int _socket;
        char error_msg[42];

    public:
        CloseException(const int socket, const int err_num);
};




/**
    Thrown when a socket fails to be bound to a server
    @param socket socket who fails to be bound
    @param server server that socket is trying to bind to
*/
class BindException : public std::exception
{
    private:
        const int _socket;
        const sockaddr_in _server;
        char error_msg[53];
    
    public:
        BindException(const int socket, const sockaddr_in server, const int err_num);
        const char * what () const throw ();
        const int getSocket();
        const sockaddr_in getServer();
};



/**
    Thrown when socket fails to initialize
*/
class SocketInitException : public std::exception
{
    private:
        char error_msg[40];

    public:
        SocketInitException(const int err_num);
        const char * what() throw();
};



/**
    Thrown when fails to receive data from a socket
    @param socket the socket that sent the data
*/
class ReceiveException : public std::exception
{
    private:
        const int _socket;
        char error_msg[50];

    public:
        ReceiveException(const int socket, const int err_num);
        const char * what() throw();
        const int getSocket();
};




/**
    Thrown when connection to socket from server fails
    @param socket socket that fails to connect to server
    @param server the inet socket address structure for which the socket failed to connect to
*/
class ConnectException : public std::exception
{
    private:
        const int _socket;
        const sockaddr_in _server;
        char error_msg[56];
    
    public:
        ConnectException(const int socket, const sockaddr_in server, const int err_num);
        const char * what() throw();
        const int getSocket();
        const sockaddr_in getServer();
};