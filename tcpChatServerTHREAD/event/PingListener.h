#include <iostream>
#include <string>
#include <mutex>

#include "EventListener.h"

using namespace std;

class PingListener : public EventListener
{  
    private:
        mutex lock;

    public:
        void onEventPing(EventPing* event) override
        {
            lock.unlock();
        }

        void waitForCall()
        {
            lock.lock();
        }
};