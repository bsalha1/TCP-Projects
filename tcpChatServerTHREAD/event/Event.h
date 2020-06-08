#pragma once

#include <iostream>
#include <string>

#include "../lib/TCPClient.h"
#include "../Util.h"

using namespace std;

// Sent from server to client
class Event
{
    private:
        string responseHandle; // The response title the client receives

    public:
        Event(string responseHandle)
        {
            this->responseHandle = "/event/" + responseHandle;
        }

        string getResponseHandle()
        {
            return this->responseHandle;
        }

        void sendEvent(TCPServer* server, int socket)
        {   
            char message[responseHandle.size() + 1];
            strcpy(message, responseHandle.c_str());
            server->sendData(message, strlen(message), socket);
        }
};