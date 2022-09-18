#include "IEvent.hpp"
#include "Common.hpp"

namespace event_system
{

    class QuickEvent : public IEvent
    {
    public:
        QuickEvent(std::function<void()> evt)
        {
            disEvent = evt;
        }

        void Dispatch() 
        {
            disEvent();
        }
    private:
        std::function<void()> disEvent;
    };

    EventPool::~EventPool()
    {
        for(IEvent* currentEvent : eventList)
        {
            delete currentEvent;
        }

        eventList.clear();
    }

    void EventPool::OnDispatch(std::function<void()> event)
    {
        QuickEvent* qEvent = new QuickEvent(event);

        eventList.push_back(qEvent);
    }

    void EventPool::Dispatch()
    {
        for(IEvent* currentEvent : eventList)
        {
            currentEvent->Dispatch();
        }
    }

}