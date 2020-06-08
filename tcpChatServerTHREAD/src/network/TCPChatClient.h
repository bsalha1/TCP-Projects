#pragma once

#include <thread>
#include <list>
#include <mutex>

#include "../../lib/TCPClient.h"

#include "../event/EventListener.h"
#include "../event/EventListenerHandler.h"

#include "../command/CommandAbstract.h"
#include "../command/CommandHandler.h"

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
        mutex lock;
        EventListenerHandler* eventHandler;
        CommandHandler* commandHandler;

        void getServerDataThread();
        void getInputThread();

    public:
        TCPChatClient(char* address, unsigned short port);

        EventListenerHandler* getEventListenerHandler();

        CommandHandler* getCommandHandler();

        int getLocalSocket();

        void registerCommand(CommandAbstract* command);

        void registerEventListener(EventListener* listener);

        void unregisterEventListener(EventListener* listener);

        void wait();
};