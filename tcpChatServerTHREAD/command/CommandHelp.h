#include <string>
#include <list>
#include <iostream>

#include "CommandAbstract.h"

using namespace std;

class CommandHelp : public CommandAbstract
{
    private:
        CommandHandler* handler;

    public:
        CommandHelp(CommandHandler* handler) : CommandAbstract("help", "displays list of commands and their descriptions")
        {
            this->handler = handler;
        }

        void execute(list<string> args) override
        {
            cout << "Commands:" << endl;
            for(CommandAbstract* command : handler->getCommands())
            {
                cout << "\t" + command->getLabel() + ": " + command->getDescription() << endl;
            }
        }
};

