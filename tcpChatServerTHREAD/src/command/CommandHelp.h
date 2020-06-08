#pragma once

#include <string>
#include <list>

#include "CommandHandler.h"
#include "CommandAbstract.h"

using namespace std;

class CommandHelp : public CommandAbstract
{
    private:
        CommandHandler* handler;

    public:
        CommandHelp(CommandHandler* handler);

        void execute(list<string> args) override;
};