#include <string.h>
#include <signal.h>

#include <sys/select.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "network/TCPChatServer.h"

void exitHandler(int signal);
TCPChatServer * tcpChatServer;

int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "[FAIL] Must specify 2 arguments, address and port.\n");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, exitHandler);
    char * address = argv[1];
    short port = (short) atoi(argv[2]);
    tcpChatServer = new TCPChatServer(address, port);
    
    tcpChatServer->runServer();

    exit(EXIT_SUCCESS);
}

void exitHandler(int signal)
{
    tcpChatServer->~TCPChatServer();
}
    