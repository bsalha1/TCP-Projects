#include <iostream>
#include <string>
#include <list>

#include "EventListener.h"
#include "../Util.h"

template<typename Base, typename T>
static inline bool instanceof(const T*)
{
   return is_base_of<Base, T>::value;
}

class EventListenerHandler
{
    private:
        list<EventListener*> listeners;

    public:
        void registerEventListener(EventListener* listener)
        {
            listeners.push_back(listener);
        }

        void unregisterEventListener(EventListener* listener)
        {
            listeners.remove(listener);
        }

        void onPingEvent(EventPing* event)
        {
            for(EventListener* listener : listeners)
            {
                cout << "Dispatching EventPing" << endl;
                listener->onEventPing((EventPing*)event);
            }
        }
};