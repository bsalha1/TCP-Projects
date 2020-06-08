#include <string>

#include "EventPing.h"

using namespace std;

// For client
EventPing::EventPing(chrono::system_clock::time_point pongTimestamp) : Event("Ping")
{
    this->pongTimestamp = pongTimestamp;
}

// For server to dispatch; the pongTimestamp amount is client-side, so server shouldn't use the first constructor 
EventPing::EventPing() : Event("Ping")
{
    
}

chrono::system_clock::time_point EventPing::getPongTimeStamp()
{
    return this->pongTimestamp;
}