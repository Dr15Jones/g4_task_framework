#ifndef EVENTSOURCE_HPP
#define EVENTSOURCE_HPP

#include <optional>
#include <atomic>

class EventSource
{
public:
    EventSource(int numEvents) : numEvents_(numEvents) {}
    std::optional<int> nextEvent()
    {
        if (currentEvent_ < numEvents_)
        {
            return currentEvent_++;
        }
        else
        {
            return {};
        }
    }

private:
    const int numEvents_;
    std::atomic<int> currentEvent_ = 0;
};


#endif