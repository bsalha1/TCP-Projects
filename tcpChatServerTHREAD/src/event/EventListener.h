#pragma once

#include <iostream>
#include <string>

#include "EventPing.h"

using namespace std;

class EventListener
{
    public:
        virtual void onEventPing(EventPing* event) = 0;
};