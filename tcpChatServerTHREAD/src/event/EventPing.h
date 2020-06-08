#pragma once

#include <chrono>
#include "Event.h"

using namespace std;

class EventPing : public Event
{
    private:
        chrono::system_clock::time_point pongTimestamp;
    
    public:
        // For client
        EventPing(chrono::system_clock::time_point pongTimestamp);

        // For server to dispatch; the pongTimestamp amount is client-side, so server shouldn't use the first constructor 
        EventPing();

        chrono::system_clock::time_point getPongTimeStamp();
};