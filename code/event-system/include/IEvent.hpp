#ifndef _IEVENT_HPP
#define _IEVENT_HPP

#include <map>
#include <vector>
#include <functional>

namespace event_system
{
    
    class IEvent
    {
    public:
        virtual ~IEvent() = default;
        virtual void Dispatch() = 0;
    };

    class EventPool
    {
    public:
        ~EventPool();

        void Dispatch();
        void OnDispatch(std::function<void()> event);
    private:
        std::vector<IEvent*> eventList;
    };

}

#endif