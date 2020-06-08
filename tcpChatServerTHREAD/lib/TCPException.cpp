#include "TCPException.h"

using namespace std;

SendException::SendException(const int socket, const int err_num) : _socket(socket) 
{
    sprintf(error_msg, "Failed to send data to socket %d: %s\n", _socket, strerror(err_num));
}

const char * SendException::what () const throw ()
{
    return error_msg;
}

const int SendException::getSocket()
{
    return _socket;
}




AcceptException::AcceptException(const int accepted_socket, const int acceptor_socket, const int err_num) : _accepted_socket(accepted_socket), _acceptor_socket(acceptor_socket)
{
    sprintf(error_msg, "Failed to accept socket %d on acceptor socket %d: %s\n", accepted_socket, acceptor_socket, strerror(err_num));
}

const char * AcceptException::what () const throw ()
{
    return error_msg;
}

const int AcceptException::getAcceptedSocket()
{
    return _accepted_socket;
}

const int AcceptException::getAcceptorSocket()
{
    return _acceptor_socket;
}




ListenException::ListenException(const int socket, const int err_num) : _socket(socket)
{
    sprintf(error_msg, "Failed to listen on socket %d: %s\n", socket, strerror(err_num));
}

const char * ListenException::what () const throw ()
{
    return error_msg;
}

const int ListenException::getSocket()
{
    return _socket;
}




CloseException::CloseException(const int socket, const int err_num) : _socket(socket)
{
    sprintf(error_msg, "Failed to listen on socket %d: %s\n", socket, strerror(err_num));
}




BindException::BindException(const int socket, const sockaddr_in server, const int err_num) : _socket(socket), _server(server)
{
    sprintf(error_msg, "Failed to bind socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(err_num));
}

const char * BindException::what () const throw ()
{
    return error_msg;
}

const int BindException::getSocket()
{
    return _socket;
}

const sockaddr_in BindException::getServer()
{
    return _server;
}




SocketInitException::SocketInitException(const int err_num)
{
    sprintf(error_msg, "Failed to initialize socket: %s\n", strerror(err_num));
}

const char * SocketInitException::what() throw()
{
    return error_msg;
}




ReceiveException::ReceiveException(const int socket, const int err_num) : _socket(socket)
{
    sprintf(error_msg, "Failed to receive data from socket %d: %s\n", socket, strerror(err_num));
}

const char * ReceiveException::what() throw()
{ 
    return error_msg; 
}

const int ReceiveException::getSocket()
{ 
    return _socket; 
}





ConnectException::ConnectException(const int socket, const sockaddr_in server, const int err_num) : _socket(socket), _server(server)
{
    sprintf(error_msg, "Failed to connect socket %d to server %s:%d: %s\n", socket, inet_ntoa(server.sin_addr), ntohs(server.sin_port), strerror(err_num));
}

const char * ConnectException::what() throw()
{
    return error_msg;
}

const int ConnectException::getSocket()
{
    return _socket;
}

const sockaddr_in ConnectException::getServer()
{
    return _server;
}
