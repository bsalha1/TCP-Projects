#include <string>
#include <malloc.h>

#include "Request.h"
#include "../util/Util.h"

using namespace std;

Request::Request(string requestHandle)
{
    this->requestHandle = "/request/" + requestHandle;
}

void Request::sendRequest(TCPClient* client, int socket)
{
    char* message = (char *) malloc((requestHandle.size() + 1) * sizeof(char));
    strcpy(message, requestHandle.c_str());
    client->sendData(message, strlen(message), socket);
    free(message);
}