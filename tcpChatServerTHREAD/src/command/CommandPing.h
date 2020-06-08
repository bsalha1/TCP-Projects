#pragma once

#include <iostream>
#include <string>
#include <list>

#include <math.h>

#include "CommandAbstract.h"
#include "../network/TCPChatClient.h"

using namespace std;

class CommandPing : public CommandAbstract
{
    private:
        TCPChatClient* client;
        int socket;

        double getPing(int socket);

    public:
        CommandPing(TCPChatClient* client, int socket);

        void execute(list<string> args) override;
};