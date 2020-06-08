#include <iostream>
#include <string>
#include <mutex>

#include "PingListener.h"

using namespace std;

void PingListener::onEventPing(EventPing* event)
{
    lock.unlock();
}

void PingListener::waitForCall()
{
    lock.lock();
}