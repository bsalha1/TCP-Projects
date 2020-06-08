#include <malloc.h>

#include "Event.h"

using namespace std;

Event::Event(string responseHandle)
{
    this->responseHandle = "/event/" + responseHandle;
}

string Event::getResponseHandle()
{
    return this->responseHandle;
}

void Event::sendEvent(TCPServer* server, int socket)
{   
    char* message = (char *) malloc((responseHandle.size() + 1) * sizeof(char));
    strcpy(message, responseHandle.c_str());
    server->sendData(message, strlen(message), socket);
}