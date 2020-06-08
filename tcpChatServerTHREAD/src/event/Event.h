#pragma once

#include <iostream>
#include <string>

#include "../../lib/TCPServer.h"

using namespace std;

// Sent from server to client
class Event
{
    private:
        string responseHandle; // The response title the client receives

    public:
        Event(string responseHandle);

        string getResponseHandle();

        void sendEvent(TCPServer* server, int socket);
};