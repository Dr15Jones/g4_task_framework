#include "EventProcessor.hpp"
#include "Module.hpp"
#include <iostream>
#include <cassert>
#include <format>

namespace {
class ModuleA : public Module {
public:
    void processEvent(Event& event) override {
        event.setData("ModuleA_Processed", true);
    }
};
class ModuleB : public Module {
public:
    void processEvent(Event& event) override {
        auto v = event.getData("ModuleA_Processed");
        assert(v.has_value() && std::any_cast<bool>(v) == true);
        event.setData("ModuleB_Processed", true);
    }
};

class ModuleC : public Module {
public:
    void processEvent(Event& event) override {
        auto v = event.getData("ModuleB_Processed");
        assert(v.has_value() && std::any_cast<bool>(v) == true);
        event.setData("ModuleC_Processed", true);
    }
};
class ModuleD : public Module {
public:
    void processEvent(Event& event) override {
        auto v = event.getData("ModuleC_Processed");
        assert(v.has_value() && std::any_cast<bool>(v) == true);
        std::cout << std::format("Event {} in loop {} processed successfully.\n", event.id(), event.loopID()) << std::flush;
    }
};
} // namespace

int main(int argc, char *argv[])
{
    int numEvents = 100000;
    int numConcurrentLoops = 4;
    int nThreads = 4;

    EventProcessor processor(numEvents, numConcurrentLoops, nThreads);
    ModuleA moduleA;
    ModuleB moduleB;
    ModuleC moduleC;
    ModuleD moduleD;
    processor.addModuleToAllLoops(&moduleA);
    processor.addModuleToAllLoops(&moduleB);
    processor.addModuleToAllLoops(&moduleC);
    processor.addModuleToAllLoops(&moduleD);
    processor.run();

    return 0;
}