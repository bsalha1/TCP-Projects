#include "CommandAbstract.h"

using namespace std;

CommandAbstract::CommandAbstract(string label, string description)
{
    this->label = label;
    this->description = description;
}

string CommandAbstract::getLabel()
{
    return this->label;
}

string CommandAbstract::getDescription()
{
    return this->description;
}