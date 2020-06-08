#pragma once

#include <string>
#include <list>

#include "CommandAbstract.h"

using namespace std;

class CommandQuit : public CommandAbstract
{
    public:
        CommandQuit();

        void execute(list<string> args) override;
};