#pragma once

#include <iostream>
#include <string>

#include "EventPing.h"
#include "Event.h"

class EventListener
{
    public:
        virtual void onEventPing(EventPing* event)
        {

        }
};