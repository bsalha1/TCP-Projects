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

#include "types.h"
#include "../tcpLib/tcpServer.h"

using namespace std;

class TCPChatServer : protected TCPServer
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
    /*
        Constructor: creates the master socket to which clients will connect to 
    */
    TCPChatServer(char * address, unsigned short port) : TCPServer(address, port)
    {
        try
        {
            masterSocket = makeSocket();
            bindSocket(masterSocket);
            listenOnSocket(masterSocket, 5);
        }
        catch(exception& e)
        {
            fprintf(stderr, "%s", e.what());
            exit(EXIT_FAILURE);
        }
        this->isRunning = true;
    }

    TCPChatServer()
    {

    }

    /*
        Deconstructor: joins all threads and closes the master socket
    */
    ~TCPChatServer()
    {
        this->isRunning = false;
        distributeData(SERVER_QUIT, sizeof(SERVER_QUIT));
        fprintf(stderr, "[INFO] TCPChatServer closed\n");
    }

    // Listens on given server port for messages. Creates new thread for each client and sends each client and the server every message.
    void runServer();

    // Sends all chat and client connection messages to clients
    void distributeData(char * data, size_t size);

    // The thread created for each socket, first argument is the value of the socket and the second argument is the id
    void socketThread(int socket, int id);

    map<int, thread*> getThreads() noexcept { return this->threads; }
};

void TCPChatServer::runServer()
{
    unsigned int id = 0;
    int clientSocket;
    sockaddr_in clientAddress;
    socklen_t clientSocketLength = sizeof(clientAddress);
    while(this->isRunning == true)
    {
        // Put a new socket into the client sockets map
        try
        {   
            clientSocket = acceptClient(masterSocket, (struct sockaddr *) &clientAddress, &clientSocketLength); // Pauses here
        }
        catch(exception& e)
        {
            if(this->isRunning == false) return;
            fprintf(stdout, "%s", e.what());
            continue;
        }
        
        // Start a new thread for the client socket
        mut.lock();
        threads.insert(pair<int, thread*>(
            id, new thread(&TCPChatServer::socketThread, this, clientSocket, id)));
        mut.unlock();
        id++;
    }
    return;
}



void TCPChatServer::distributeData(char * data, size_t size)
{
    mut.lock();
    map<int, sockaddr_in>::iterator clientSocket = clientSockets.begin();
    while(clientSocket != clientSockets.end())
    {
        sendData(data, size, clientSocket->first);
        clientSocket++;
    }
    mut.unlock();
    return;
}


/**
 * Thread for each client 
 **/
void TCPChatServer::socketThread(int socket, int id)
{
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Thread %d started for socket: %d\n", id, socket);
    #endif
    
    char username[max_username], message[max_message];
    char formattedMessage[max_message + max_username + 2], quitMessage[max_username + 15], joinMessage[max_username + 12];
    int sizeRead;

    // Create username, quit and join messages
    sprintf(username, "Client-%d", socket);
    sprintf(quitMessage, "%s has disconnected", username);
    sprintf(joinMessage, "%s has connected", username);

    // Send all clients the join message
    distributeData(joinMessage, sizeof(joinMessage));

    stdoutMut.lock();
    fprintf(stdout, "%s\n", joinMessage);
    stdoutMut.unlock();

    /*
        Read data from socket and send data to all clients
    */
    while(true)
    {
        sizeRead = read(socket, message, sizeof(message)); // Thread pauses here
        if((sizeRead == 0) || (sizeRead == -1) || !strcmp(message, CLIENT_QUIT))  // size = 0 -> empty msg, size = -1 -> read fail
        {
            break;
        }
        message[sizeRead] = '\0'; // Terminates string (flushes string)

        // Format the message
        sprintf(formattedMessage, "%s: %s", username, message);

        // Send all clients the message
        distributeData(formattedMessage, strlen(formattedMessage));
        fprintf(stdout, "%s\n", formattedMessage);
    }

    // Deregister client
    mut.lock();
    closeSocket(socket);
    mut.unlock();
    
    // Send out quit message to clients
    distributeData(quitMessage, sizeof(quitMessage));

    stdoutMut.lock();
    fprintf(stdout, "%s\n", quitMessage);
    stdoutMut.unlock();

    mut.lock();
    threads.erase(id);
    mut.unlock();
    return;
}

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
    int port = atoi(argv[2]);
    tcpChatServer = new TCPChatServer(address, port);
    
    tcpChatServer->runServer();

    exit(EXIT_SUCCESS);
}

void exitHandler(int signal)
{
    tcpChatServer->~TCPChatServer();
}
    