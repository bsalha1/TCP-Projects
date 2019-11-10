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

/*
    Thrown when failure to send data to a socket
    @socket: socket which failed to be written to
*/
class sendException : public std::exception
{
    private:
        const int _socket;
        char * errorMsg;

    public:
        sendException(const int socket, const int errNum) : _socket(socket) 
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to send data to socket %d: %s\n", _socket, strerror(errNum));
            #else
            sprintf(errorMsg, "Failed to send data to socket %d: %s\n", _socket, strerror(errNum));
            #endif
        }

        const char * what () const throw (){ return errorMsg; }

        const int getSocket(){ return _socket; }
};



/*
    Thrown when one socket fails to accept another
    @acceptedSocket: socket to be accepted with acceptor
    @acceptorSocket: socket attempting to accept
*/
class acceptException : public std::exception
{
    private:
        const int _acceptedSocket;
        const int _acceptorSocket;
        char errorMsg[61];

    public:
        acceptException(const int acceptedSocket, const int acceptorSocket, const int errNum) : _acceptedSocket(acceptedSocket), _acceptorSocket(acceptorSocket)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to accept socket %d on acceptor socket %d: %s\n", acceptedSocket, acceptorSocket, strerror(errNum));
            #else
            sprintf(errorMsg, "Failed to accept socket %d on acceptor socket %d: %s\n", acceptedSocket, acceptorSocket, strerror(errNum));
            #endif
        }

        const char * what () const throw (){ return errorMsg; }

        const int getAcceptedSocket(){ return _acceptedSocket; }
        const int getAcceptorSocket(){ return _acceptorSocket; }
};



/*
    Thrown when socket fails to initiate listening
    @socket: socket which fails to listen
*/
class listenException : public std::exception
{
    private:
        const int _socket;
        char errorMsg[42];
    
    public:
        listenException(const int socket, const int errNum) : _socket(socket)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to listen on socket %d: %s\n", socket, strerror(errNum));
            #else
            sprintf(errorMsg, "Failed to listen on socket %d: %s\n", socket, strerror(errNum));
            #endif
        }
        
        const char * what () const throw (){ return errorMsg; }

        const int getSocket(){ return _socket; }
};



/*
    Thrown when socket fails to lose under a close() call
    @socket: socket which failed to close
*/
class closeException : public std::exception
{
    private:
        const int _socket;
        char errorMsg[42];

    public:
        closeException(const int socket, const int errNum) : _socket(socket)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to listen on socket %d: %s\n", socket), strerror(errNum);
            #else
            sprintf(errorMsg, "Failed to listen on socket %d: %s\n", socket, strerror(errNum));
            #endif
        }
};




/*
    Thrown when a socket fails to be bound to a server
    @socket: socket who fails to be bound
    @server: server that socket is trying to bind to
*/
class bindException : public std::exception
{
    private:
        const int _socket;
        const sockaddr_in _server;
        char errorMsg[53];
    
    public:
        bindException(const int socket, const sockaddr_in server, const int errNum) : _socket(socket), _server(server)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to bind socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(errNum));
            #else
            sprintf(errorMsg, "Failed to bind socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(errNum));
            #endif
        }

        const char * what () const throw (){ return errorMsg; }

        const int getSocket(){ return _socket; }
        const sockaddr_in getServer(){ return _server; }
};



/*
    Thrown when socket fails to initialize
*/
class socketInitException : public std::exception
{
    private:
        char errorMsg[40];

    public:
        socketInitException(const int errNum)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to initialize socket: %s\n", strerror(errNum));    
            #else
            sprintf(errorMsg, "Failed to initialize socket: %s\n", strerror(errNum)); 
            #endif
        }

        const char * what() throw(){ return errorMsg; }
};



/*
    Thrown when fails to receive data from a socket
    @socket: the socket that sent the data
*/
class receiveException : public std::exception
{
    private:
        const int _socket;
        char errorMsg[50];

    public:
        receiveException(const int socket, const int errNum) : _socket(socket)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to receive data from socket %d: %s\n", socket, strerror(errNum));
            #else
            sprintf(errorMsg, "Failed to receive data from socket %d: %s\n", socket, strerror(errNum));
            #endif
        }

        const char * what() throw(){ return errorMsg; }

        const int getSocket(){ return _socket; }
};




/*
    Thrown when connection to socket from server fails
    @socket: socket that fails to connect to server
    @server: the inet socket address structure for which the socket failed to connect to
*/
class connectException : public std::exception
{
    private:
        const int _socket;
        const sockaddr_in _server;
        char errorMsg[56];
    
    public:
        connectException(const int socket, const sockaddr_in server, const int errNum) : _socket(socket), _server(server)
        {
            #ifdef FORMATTED_ERROR
            sprintf(errorMsg, "[FAIL] Failed to connect socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(errNum));
            #else
            sprintf(errorMsg, "Failed to connect socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(errNum));
            #endif
        }

        const char * what() throw(){ return errorMsg; }

        const int getSocket(){ return _socket; }
        const sockaddr_in getServer(){ return _server; }
};