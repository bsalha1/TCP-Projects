#include <iostream>

#include "CommandHelp.h"

using namespace std;

CommandHelp::CommandHelp(CommandHandler* handler) : CommandAbstract("help", "displays list of commands and their descriptions")
{
    this->handler = handler;
}

void CommandHelp::execute(list<string> args)
{
    cout << "Commands:" << endl;
    for(CommandAbstract* command : handler->getCommands())
    {
        cout << "\t" + command->getLabel() + ": " + command->getDescription() << endl;
    }
}

