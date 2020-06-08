#pragma once
#include <string>

using namespace std;

class CommandAbstract
{
    private:
        string label;
        string description;

    public:

        CommandAbstract(string label, string description)
        {
            this->label = label;
            this->description = description;
        }

        virtual void execute(list<string> args)
        {
            
        }

        
        string getLabel()
        {
            return this->label;
        }

        string getDescription()
        {
            return this->description;
        }
};
