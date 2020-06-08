#pragma once

#include <mutex>

#include "EventPing.h"
#include "EventListener.h"

using namespace std;

class PingListener : public EventListener
{  
    private:
        mutex lock;

    public:
        void onEventPing(EventPing* event) override;

        void waitForCall();
};