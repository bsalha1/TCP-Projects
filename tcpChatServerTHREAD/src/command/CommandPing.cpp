
#include "../request/RequestPing.h"
#include "../event/PingListener.h"
#include "CommandPing.h"

using namespace std;

CommandPing::CommandPing(TCPChatClient* client, int socket) : CommandAbstract("ping", "get response time from server")
{
    this->client = client;
    this->socket = socket;
}

double CommandPing::getPing(int socket)
{
    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    PingListener* listener = new PingListener();
    client->registerEventListener(listener);

    // Send request for PingEvent
    RequestPing* request = new RequestPing();
    request->sendRequest(client, socket);
    listener->waitForCall();

    high_resolution_clock::time_point end = high_resolution_clock::now();
    client->unregisterEventListener(listener);

    duration<double> time_span = end - start;
    double delay = time_span.count() * pow(10, 3);
    return delay;
}

void CommandPing::execute(list<string> args)
{
    double ping = getPing(this->socket);
    printf("%lf ms\n", ping);
}