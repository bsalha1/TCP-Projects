#pragma once

#include <iostream>
#include <list>
#include "CommandAbstract.h"

using namespace std;

class CommandHandler
{
    private:
        list<CommandAbstract*> commands;

    public:
        void registerCommand(CommandAbstract* command);

        void unregisterCommand(CommandAbstract* command);

        CommandHandler();

        void handleCommand(string context);

        list<CommandAbstract*> getCommands();
};