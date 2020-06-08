#pragma once

#include <iostream>
#include <string>

#include "Event.h"

using namespace std;

class EventPing : public Event
{
    private:
        chrono::system_clock::time_point pongTimestamp;
    
    public:
        // For client
        EventPing(chrono::system_clock::time_point pongTimestamp) : Event("Ping")
        {
            this->pongTimestamp = pongTimestamp;
        }

        // For server to dispatch; the pongTimestamp amount is client-side, so server shouldn't use the first constructor 
        EventPing() : Event("Ping")
        {
            
        }

        chrono::system_clock::time_point getPongTimeStamp()
        {
            return this->pongTimestamp;
        }
};