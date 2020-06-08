#include <iostream>
#include <thread>
#include <string>
#include <list>
#include <iterator>
#include <mutex>
#include <exception>
#include <chrono>

#include <signal.h>

#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "TCPChatClient.h"

void exitHandler(int signal);
int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "[FAIL] Must enter two arguments; address and port.\n");
        return EXIT_FAILURE;
    }
    signal(SIGINT, exitHandler);

    char * address = argv[1];
    int port = atoi(argv[2]);

    TCPChatClient* client = new TCPChatClient(address, port);

    exit(EXIT_SUCCESS);
}

void exitHandler(int signal)
{
    exit(EXIT_SUCCESS);
}