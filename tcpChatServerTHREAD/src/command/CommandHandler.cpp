#include "CommandHandler.h"
#include "../util/Util.h"

using namespace std;

void CommandHandler::registerCommand(CommandAbstract* command)
{
    commands.push_back(command);
}

void CommandHandler::unregisterCommand(CommandAbstract* command)
{
    commands.remove(command);
}

CommandHandler::CommandHandler()
{

}

void CommandHandler::handleCommand(string context)
{
    string label = context.substr(0, context.find_first_of(' '));
    
    for(CommandAbstract* command : commands)
    {
        if(label.compare(command->getLabel()) == 0)
        {
            command->execute(Util::split(context, ' '));
            return;
        }
    }
    cerr << "Unknown command: " + label << endl;
}

list<CommandAbstract*> CommandHandler::getCommands()
{
    return this->commands;
}