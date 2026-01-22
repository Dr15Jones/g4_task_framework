#include "EventProcessor.hpp"
#include "ReentrantModule.hpp"
#include <iostream>
#include <format>

namespace {
  class ModuleA : public AsyncModule {
  public:
    void processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& event) const final {
        iGroup.run([&event, group = &iGroup, handle = std::move(iHandle)]() {
            event.setData("ModuleA_Processed", true);
            // Continue processing by calling the handle
            group->run(std::move(const_cast<tbb::task_handle&>(handle)));
        }); 
    }
  };
  class ModuleB : public ReentrantModule {
  public:
    explicit ModuleB(std::optional<int> throwExceptionOnEvent ): throwExceptionOnEvent_(throwExceptionOnEvent) {}
    void processEvent(Event& event) const final {
      if (throwExceptionOnEvent_.has_value() and event.id() == *(throwExceptionOnEvent_)) {
        throw std::runtime_error("test failure");
      }
      auto v = event.getData("ModuleA_Processed");
      if( not (v.has_value() && std::any_cast<bool>(v) == true)) {
        throw std::runtime_error("ModuleA_Processed data missing or incorrect in ModuleB");
      }
      event.setData("ModuleB_Processed", true);
    }
    private:
      std::optional<int> throwExceptionOnEvent_;
  };

  class ModuleC : public ReentrantModule {
  public:
    void processEvent(Event& event) const final {
      auto v = event.getData("ModuleB_Processed");
      if( not (v.has_value() && std::any_cast<bool>(v) == true)) {
        throw std::runtime_error("ModuleB_Processed data missing or incorrect in ModuleC");
      }
      event.setData("ModuleC_Processed", true);
    }
  };
  class ModuleD : public ReentrantModule {
  public:
    void processEvent(Event& event) const final {
      auto v = event.getData("ModuleC_Processed");
      if( not (v.has_value() && std::any_cast<bool>(v) == true)) {
        throw std::runtime_error("ModuleC_Processed data missing or incorrect in ModuleD");
      }
      std::cout << std::format("Event {} in loop {} processed successfully.\n", event.id(), event.loopID())
                << std::flush;
    }
  };
}  // namespace

int main(int argc, char* argv[]) {
  int numEvents = 100000;
  int numConcurrentLoops = 4;
  int nThreads = 4;

  EventProcessor processor(numEvents, numConcurrentLoops, nThreads);
  ModuleA moduleA;
  std::optional<int> throwExceptionOnEvent;
  // Uncomment the next line to test exception handling
  // throwExceptionOnEvent = 42;
  ModuleB moduleB(throwExceptionOnEvent);
  ModuleC moduleC;
  ModuleD moduleD;
  processor.addModuleToAllLoops(&moduleA);
  processor.addModuleToAllLoops(&moduleB);
  processor.addModuleToAllLoops(&moduleC);
  processor.addModuleToAllLoops(&moduleD);
  processor.run();

  return 0;
}
