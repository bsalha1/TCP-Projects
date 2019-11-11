#include <iostream>
#include <thread>
#include <string>
#include <list>
#include <iterator>
#include <mutex>
#include <exception>

#include <signal.h>

#include <sys/socket.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tcpLib/tcpClient.h"

void getServerDataThread();
void getInputThread();
void exitHandler(int signal);

int isRunning;
int localSocket;
TCPClient * tcpClient;
thread * getInput;
thread * getServerData;
mutex mut;
mutex stdoutMut;

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, "[FAIL] Must enter two arguments; address and port.\n");
        return EXIT_FAILURE;
    }
    signal(SIGINT, exitHandler);

    char * address;
    char * port;
    char * quitMsg = "CLIENT_CONFIRM_QUIT";
    char filename[64];
    char message[max_message];
    sockaddr_in server;

    address = argv[1];
    port = argv[2];
    tcpClient = new TCPClient(address, atoi(port));

    server = tcpClient->getServer();
    localSocket = tcpClient->makeSocket();

    // Try connecting to server ...
    try
    {
        tcpClient->connectToServer(localSocket);
    }
    catch(exception& e)
    {
        fprintf(stdout, "%s", e.what());
    }

    isRunning = true;
    getInput = new thread(getInputThread);
    getServerData = new thread(getServerDataThread);

    // Continue when server closes
    getServerData->join();
    tcpClient->sendData(quitMsg, strlen(quitMsg), localSocket);
    mut.lock();
    tcpClient->closeSocket(localSocket);
    mut.unlock();

    stdoutMut.lock();
    fprintf(stdout, "Server closed, press enter to exit.\n");
    stdoutMut.unlock();

    isRunning = false;
    getInput->join();

    exit(EXIT_SUCCESS);
}



/*
    Get client input then send to server
*/
void getInputThread()
{
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Input thread initialized\n");
    #endif
    char message[max_message];
    while(true)
    {
        read(STDIN_FILENO, message, sizeof(message)); // Thread pauses until input is read
        message[strcspn(message, "\n")] = 0; // Cut off \n
        if((strcmp(message, "/quit") == 0) || !isRunning)
        {
            break;
        }

        try
        {
            tcpClient->sendData(message, strlen(message), localSocket);
        }
        catch(exception &e)
        {
            stdoutMut.lock();
            fprintf(stdout, "%s", e.what());
            stdoutMut.unlock();
            break;
        }
    }
}



/*
    Get data of server
*/
void getServerDataThread()
{
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Server data thread initialized\n");
    #endif
    char buffer[max_message + max_username + 2];
    int sizeData;
    while(true)
    {
        try
        {
            sizeData = tcpClient->receiveData(buffer, sizeof(buffer), localSocket); // Pause
        }
        catch(exception& e)
        {
            stdoutMut.lock();
            fprintf(stdout, "%s", e.what());
            stdoutMut.unlock();
            break;
        }
        
        if(strcmp(buffer, "SERVER_CLOSE\n") == 0) // If server sends closing message
        {
            break;
        }
        fprintf(stdout, "%s\n", buffer);
    }
    return;
}



void exitHandler(int signal)
{
    exit(EXIT_SUCCESS);
}