#pragma once

#include <iostream>
#include <string>
#include <list>

#include "EventListener.h"
#include "EventPing.h"

using namespace std;

class EventListenerHandler
{
    private:
        list<EventListener*> listeners;

    public:
        void registerEventListener(EventListener* listener);

        void unregisterEventListener(EventListener* listener);

        void onPingEvent(EventPing* event);
};