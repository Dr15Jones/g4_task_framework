#ifndef MODULE_HPP
#define MODULE_HPP

#include "Event.hpp"

class Module {
public:
    virtual ~Module() = default;
    virtual void processEvent(Event& event) = 0;
};

#endif
