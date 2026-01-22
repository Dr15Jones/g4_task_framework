#include "EventProcessor.hpp"
#include "ReentrantModule.hpp"
#include "LoopCloneModule.hpp"
#include <iostream>
#include <format>

namespace {
  class ModuleA : public AsyncModule {
  public:
    void processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& event) const final {
      auto handlePtr = std::make_unique<tbb::task_handle>(std::move(iHandle));
      iGroup.run([&event, group = &iGroup, handlePtr = std::move(handlePtr)]() {
        event.setData("ModuleA_Processed", true);
        // Continue processing by calling the handle
        group->run(std::move(*(handlePtr)));
      });
    }
  };
  class ModuleB : public ReentrantModule {
  public:
    explicit ModuleB(std::optional<int> throwExceptionOnEvent) : throwExceptionOnEvent_(throwExceptionOnEvent) {}
    void processEvent(Event& event) const final {
      if (throwExceptionOnEvent_.has_value() and event.id() == *(throwExceptionOnEvent_)) {
        throw std::runtime_error("test failure");
      }
      auto v = event.getData("ModuleA_Processed");
      if (not(v.has_value() && std::any_cast<bool>(v) == true)) {
        throw std::runtime_error("ModuleA_Processed data missing or incorrect in ModuleB");
      }
      event.setData("ModuleB_Processed", true);
    }

  private:
    std::optional<int> throwExceptionOnEvent_;
  };

  class ModuleC {
  public:
    void processEvent(Event& event) {
      auto v = event.getData("ModuleB_Processed");
      if (not(v.has_value() && std::any_cast<bool>(v) == true)) {
        throw std::runtime_error("ModuleB_Processed data missing or incorrect in ModuleC");
      }
      event.setData("ModuleC_Processed", true);
      ++count_;
      loopID_ = event.loopID();
    }

    ~ModuleC() { std::cout << std::format("ModuleC in loop {} processed {} events.\n", loopID_, count_) << std::flush; }

  private:
    int count_{0};
    int loopID_{-1};
  };
  using LoopCloneModuleC = LoopCloneModule<ModuleC>;
  class ModuleD : public ReentrantModule {
  public:
    void processEvent(Event& event) const final {
      auto v = event.getData("ModuleC_Processed");
      if (not(v.has_value() && std::any_cast<bool>(v) == true)) {
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
  LoopCloneModuleC moduleC(numConcurrentLoops);
  ModuleD moduleD;
  processor.addModuleToAllLoops(&moduleA);
  processor.addModuleToAllLoops(&moduleB);
  processor.addModuleToAllLoops(&moduleC);
  processor.addModuleToAllLoops(&moduleD);
  processor.run();

  return 0;
}
