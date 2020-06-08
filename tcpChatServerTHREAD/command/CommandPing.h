#pragma once

#include <iostream>
#include <string>
#include <list>

#include "CommandAbstract.h"
#include "../Util.h"
#include "../request/Request.h"
#include "../request/RequestPing.h"
#include "../TCPChatClient.h"

using namespace std;

class CommandPing : public CommandAbstract
{
    private:
        TCPChatClient* client;
        int socket;

        // static int getPing(TCPChatClient* client, int socket)
        // {
        //     using namespace std::chrono;
        //     high_resolution_clock::time_point start = high_resolution_clock::now();

        //     PingListener* listener = new PingListener();
        //     client->getEventListenerHandler()->registerEventListener(listener);

        //     // Send request for PingEvent
        //     RequestPing* request = new RequestPing();
        //     request->sendRequest(client, socket);
        //     listener->waitForCall();

        //     high_resolution_clock::time_point end = high_resolution_clock::now();
        //     cout << (end - start).count() + "ms" << endl;

        //     client->getEventListenerHandler()->unregisterEventListener(listener);

        //     return 0;
        // }

    public:
        CommandPing(TCPChatClient* client, int socket) : CommandAbstract("ping", "get response time from server")
        {
            this->client = client;
            this->socket = socket;
        }

        void execute(list<string> args) override
        {
            // int ping = getPing(this->client, this->socket);
            // cout << "Ping: " +  ping << endl;
        }
};