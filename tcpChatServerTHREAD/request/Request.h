#pragma once

#include <iostream>
#include <string>

#include "../Util.h"
#include "../lib/TCPClient.h"

using namespace std;

class Request
{
    private:
        string requestHandle;

    public:
        Request(string requestHandle)
        {
            this->requestHandle = "/request/" + requestHandle;
        }
        
        void sendRequest(TCPClient* client, int socket)
        {
            char message[requestHandle.size() + 1];
            strcpy(message, requestHandle.c_str());
            client->sendData(message, strlen(message), socket);
        }
};