#include "max.h"
#include "TCPChatServer.h"

using namespace std;

TCPChatServer::TCPChatServer(char * address, unsigned short port) : TCPServer(address, port)
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

TCPChatServer::TCPChatServer()
{

}

TCPChatServer::~TCPChatServer()
{
    this->isRunning = false;
    // distributeData(SERVER_QUIT, sizeof(SERVER_QUIT));
    fprintf(stderr, "[INFO] TCPChatServer closed\n");
}

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
    char formattedMessage[max_message + max_username + 2], quitMessage[max_username + 17], joinMessage[max_username + 14];
    ssize_t sizeRead;

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
        if((sizeRead == 0) || (sizeRead == -1))  // size = 0 -> empty msg, size = -1 -> read fail
        {
            break;
        }
        message[sizeRead] = '\0'; // Terminates string (flushes string)

        // If request received
        if(message[0] == '/')
        {
            printf("Received Request: %s\n", message);
            if(strcmp(message, "/request/Ping") == 0)
            {   
                EventPing* event = new EventPing();
                event->sendEvent(this, socket);
                cout << "Dispatched Event: " + event->getResponseHandle() << endl;
            }
            else
            {
                printf("Unknown Request");
            }
            
            continue;
        }

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

map<int, thread*> TCPChatServer::getThreads()
{
    return this->threads;
}