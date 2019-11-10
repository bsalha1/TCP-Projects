#include <iostream>
#include <list>

#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/time.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tcpLib/tcp_server.h"

#define MAX_CLIENTS 10

using namespace std;

class TCPChatServer
{
private:
    TCPServer tcpServer;
    int masterSocket;
    list<int> clientSockets;
    fd_set readfds;
public:
    TCPChatServer(TCPServer tcpServer);
    ~TCPChatServer();
    void runServer();
    int disconnectSocket(int socket);
};


/*
    Constructor
*/
TCPChatServer::TCPChatServer(TCPServer tcpServer)
{
    this->tcpServer = tcpServer;
    this->masterSocket = tcpServer.makeSocket();
    if(masterSocket == -1)
    {
        exit(EXIT_FAILURE);
    }
    if(tcpServer.bindSocket(masterSocket) == -1)
    {
        exit(EXIT_FAILURE);
    }
    if(tcpServer.listenOnSocket(masterSocket, 10) == -1)
    {
        exit(EXIT_FAILURE);
    }
}



/*
    Deconstructor
*/
TCPChatServer::~TCPChatServer()
{

}

void TCPChatServer::runServer()
{
    int newSocket;
    int maxSocket = masterSocket;
    list<int>::iterator clientSocket;
    char username[32][MAX_CLIENTS + 12], password[256][MAX_CLIENTS + 12];
    char message[2048];

    while(1)
    {
        // Clear socket set
        FD_ZERO(&readfds);

        // Add master to set
        FD_SET(masterSocket, &readfds);

        clientSocket = clientSockets.begin();
        while(clientSocket != clientSockets.end())
        {
            // If socket is a socket, add it to the set ... //
            FD_SET(*clientSocket, &readfds);


            // If the socket is a bigger value than the max, change the max ... //
            if(*clientSocket > maxSocket)
            {
                maxSocket = *clientSocket;
            }
            clientSocket++;
        }



        // Monitor the sockets for reading ... //
        if(select(maxSocket + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            fprintf(stderr, "[FAIL] Select error: %s\n", strerror(errno));
        }



        // If master socket exists ... //
        if(FD_ISSET(masterSocket, &readfds))
        {
            // Gets a new socket being connected to the master socket
            newSocket = tcpServer.acceptClient(masterSocket);

            // Add new socket to list
            clientSockets.push_back(newSocket);
            fprintf(stderr, "[OK] Adding new socket %d to list.\n", newSocket);

            // Get username of new socket ... //
            if(read(newSocket, username[newSocket], sizeof(username[newSocket])) == -1)
            {
                
            }
            fprintf(stdout, "%s has connected\n", username[newSocket]);
        }


        // Read Messages
        clientSocket = clientSockets.begin();
        while(clientSocket != clientSockets.end())
        {
            // For clients that are connected...
            if(FD_ISSET(*clientSocket, &readfds))
            {
                if(read(*clientSocket, message, sizeof(message)) == -1)
                {
                    
                }
                else if(strcmp(message, "") == 0)
                {
                    continue;
                }
                else 
                {
                    // Print the message preceded by the username //
                    fprintf(stderr, "Client-%d: %s\n", *clientSocket, message); // To error file
                    fprintf(stdout, "%s: %s\n", username[*clientSocket], message); // To output
                    send(*clientSocket, message, strlen(message), 0);
                }
            }
            clientSocket++;
        }  
    }
}


int TCPChatServer::disconnectSocket(int socket)
{
    fprintf(stderr, "[OK] Client disconnected on socket %d\n", socket);
    fprintf(stdout, "%s has disconnected\n", username[*clientSocket]);

    tcpServer.closeSocket(*clientSocket);
    clientSocket = clientSockets.erase(clientSocket);
}

int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Server: Error, must specify 2 arguments, address and port.\n");
        return -1;
    }

    int i = 0;
    int addrlen;
    int masterSocket, maxSocket, newSocket;
    list<int> clientSockets;
    char * address = argv[1];
    int port = atoi(argv[2]);
    int opt = 1;
    struct sockaddr_in server_address;
    TCPServer tcpServer = TCPServer(address, port);
    TCPChatServer tcpChatServer = TCPChatServer(tcpServer);

    // Server runtime ... //
    


    return EXIT_SUCCESS;
}

