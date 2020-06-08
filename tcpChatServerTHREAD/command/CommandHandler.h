#include <iostream>
#include <list>
#include "CommandAbstract.h"
#include "../Util.h"

using namespace std;
class CommandHandler
{
    private:
        list<CommandAbstract*> commands;

    public:
        void registerCommand(CommandAbstract* command)
        {
            commands.push_back(command);
        }

        void unregisterCommand(CommandAbstract* command)
        {
            commands.remove(command);
        }

        CommandHandler()
        {

        }

        void handleCommand(string context)
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

        list<CommandAbstract*> getCommands()
        {
            return this->commands;
        }
};