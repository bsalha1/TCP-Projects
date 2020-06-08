#pragma once

#include <iostream>
#include <thread>
#include <list>
#include <map>
#include <iterator>
#include <mutex>

#include <string.h>
#include <signal.h>

#include <sys/select.h>
#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../util/Util.h"

#include "../event/Event.h"
#include "../event/EventPing.h"

#include "../../lib/TCPServer.h"

using namespace std;

class TCPChatServer : public TCPServer
{
    private:
        // Server Data
        int masterSocket;
        bool isRunning = false;
        mutex stdoutMut;
        mutex mut;

        // Client Data
        map<int, thread*> threads; // First is ID second is thread

    public:
    
        // Creates the master socket to which clients will connect to
        TCPChatServer(char * address, unsigned short port);

        TCPChatServer();

        // Joins all threads and closes the master socket
        ~TCPChatServer();

        // Listens on given server port for messages. Creates new thread for each client and sends each client and the server every message.
        void runServer();

        // Sends all chat and client connection messages to clients
        void distributeData(char * data, size_t size);

        // The thread created for each socket, first argument is the value of the socket and the second argument is the id
        void socketThread(int socket, int id);

        map<int, thread*> getThreads();
};