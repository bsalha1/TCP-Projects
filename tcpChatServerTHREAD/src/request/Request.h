#pragma once

#include <string>
#include "../../lib/TCPClient.h"

using namespace std;

class Request
{
    private:
        string requestHandle;

    public:
        Request(string requestHandle);

        void sendRequest(TCPClient* client, int socket);

};