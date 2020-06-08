#pragma once

#include <list>
#include <string>

using namespace std;

class CommandAbstract
{
    private:
        string label;
        string description;

    public:

        CommandAbstract(string label, string description);

        virtual void execute(list<string> args) = 0;
        
        string getLabel();

        string getDescription();
};
