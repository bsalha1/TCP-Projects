#include <iostream>
#include <string>
#include <list>

#include "CommandAbstract.h"
#include "../lib/TCPClient.h"

using namespace std;

class CommandQuit : public CommandAbstract
{
    public:
        CommandQuit() : CommandAbstract("quit", "leave the server")
        {

        }

        void execute(list<string> args) override
        {
            
        }
};