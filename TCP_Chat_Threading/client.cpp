#include <iostream>
#include <thread>
#include <string>
#include <list>
#include <iterator>
#include <mutex>
#include <condition_variable>
#include <exception>

#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tcpLib/tcpClient.h"

void getServerDataThread();
void getInputThread();
void exitHandler(int signal);

int isRunning = true;
int localSocket;
bool ready{};
list<thread *> threads;
FILE * outputFile;
TCPClient * tcpClient;
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

    thread * getInput = new thread(getInputThread);
    thread * getServerData = new thread(getServerDataThread);
    threads.push_front(getInput);
    threads.push_front(getServerData);

    exitHandler(0);
    
    return EXIT_SUCCESS;
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
    while(errno != EINTR)
    {
        read(STDIN_FILENO, message, sizeof(message)); // Thread pauses until input is read
        message[strcspn(message, "\n")] = 0; // Cut off \n
        printf("Read\n");
        if(strcmp(message, "/quit") == 0)
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
        
        if(strcmp(buffer, "Server is closing\n") == 0) // If server sends closing message
        {
            mut.lock();
            tcpClient->closeSocket(localSocket);
            mut.unlock();

            stdoutMut.lock();
            fprintf(stdout, "Server closed, press enter to exit.\n");
            stdoutMut.unlock();
            break;
        }
        fprintf(stdout, "%s\n", buffer);
    }
    return;
}



void exitHandler(int signal)
{
    char * message = "t";
    write(STDIN_FILENO, message, sizeof(message));
    threads.front()->join();
    threads.back()->join();
}