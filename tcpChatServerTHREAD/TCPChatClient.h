#pragma once

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

#include "types.h"

#include "lib/TCPClient.h"

#include "event/EventListenerHandler.h"
#include "event/PingListener.h"

#include "command/CommandHandler.h"
#include "command/CommandAbstract.h"
#include "command/CommandHelp.h"
#include "command/CommandQuit.h"
// #include "command/CommandPing.h"

using namespace std;

class TCPChatClient : public TCPClient
{
    private:
        volatile int isRunning;
        int localSocket;
        thread * getInput;
        thread * getServerData;
        mutex mut;
        mutex stdoutMut;
        EventListenerHandler* eventHandler = new EventListenerHandler();
        CommandHandler* commandHandler = new CommandHandler();

        void getServerDataThread();
        void getInputThread();

    public:
        TCPChatClient(char* address, unsigned short port) : TCPClient(address, port)
        {
            localSocket = makeSocket();
            try
            {
                connectToServer(localSocket);
            }
            catch(exception& e)
            {
                fprintf(stdout, "%s", e.what());
            }

            isRunning = true;
            getInput = new thread(&TCPChatClient::getInputThread, this);
            getServerData = new thread(&TCPChatClient::getServerDataThread, this);

            // Wait for server to close
            getServerData->join();
            sendData((void *) CLIENT_QUIT, strlen(CLIENT_QUIT), localSocket);

            mut.lock();
            closeSocket(localSocket);
            mut.unlock();

            stdoutMut.lock();
            fprintf(stdout, "Server closed, press enter to exit.\n");
            stdoutMut.unlock();

            isRunning = false;
            getInput->join();
        }

        EventListenerHandler* getEventListenerHandler()
        {
            return this->eventHandler;
        }

        CommandHandler* getCommandHandler()
        {
            return this->commandHandler;
        }
};

/*
    Get client input then send to server
*/
void TCPChatClient::getInputThread()
{
    commandHandler->registerCommand(new CommandHelp(commandHandler));
    commandHandler->registerCommand(new CommandQuit());
    commandHandler->registerCommand(new CommandPing(this, localSocket));

    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Input thread initialized\n");
    #endif
    char message[max_message];
    while(isRunning)
    {
        read(STDIN_FILENO, message, sizeof(message)); // Thread pauses until input is read
        message[strcspn(message, "\n")] = 0; // Cut off \n

        // Handle Command
        if(message[0] == '/')
        {
            // Cut off '/'
            memmove(message, message + 1, strlen(message));
            commandHandler->handleCommand(message);
            continue;
        }

        try
        {
            sendData(message, strlen(message), localSocket);
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
void TCPChatClient::getServerDataThread()
{
    #ifdef ERR_MSG
    fprintf(stderr, "[OK] Server data thread initialized\n");
    #endif
    char buffer[max_message + max_username + 2];
    int sizeData;
    while(isRunning)
    {
        try
        {
            sizeData = receiveData(buffer, sizeof(buffer), localSocket); // Client pauses here
            buffer[sizeData] = '\0';
        }
        catch(exception& e)
        {
            stdoutMut.lock();
            fprintf(stdout, "%s", e.what());
            stdoutMut.unlock();
            break;
        }

        // If event received
        if((strlen(buffer) > 0) && (buffer[0] == '/'))
        {
            printf("Received Event: %s\n", buffer);
            if(strcmp(buffer, "/event/Ping") == 0)
            {
                eventHandler->onPingEvent(new EventPing(chrono::high_resolution_clock::now()));
            }
            continue;
        }
        
        // Server close handshake
        if(strcmp(buffer, SERVER_QUIT) == 0) break;
        fprintf(stdout, "%s\n", buffer);
    }
    return;
}

class CommandPing : public CommandAbstract
{
    private:
        TCPChatClient* client;
        int socket;

        // static int getPing(TCPChatClient* client, int socket)
        // {
        //     using namespace std::chrono;
        //     high_resolution_clock::time_point start = high_resolution_clock::now();

        //     PingListener* listener = new PingListener();
        //     client->getEventListenerHandler()->registerEventListener(listener);

        //     // Send request for PingEvent
        //     RequestPing* request = new RequestPing();
        //     request->sendRequest(client, socket);
        //     listener->waitForCall();

        //     high_resolution_clock::time_point end = high_resolution_clock::now();
        //     cout << (end - start).count() + "ms" << endl;

        //     client->getEventListenerHandler()->unregisterEventListener(listener);

        //     return 0;
        // }

    public:
        CommandPing(TCPChatClient* client, int socket) : CommandAbstract("ping", "get response time from server")
        {
            this->client = client;
            this->socket = socket;
        }

        void execute(list<string> args) override
        {
            // int ping = getPing(this->client, this->socket);
            // cout << "Ping: " +  ping << endl;
        }
};