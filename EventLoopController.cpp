
#include "EventLoopController.hpp"
#include "EventSource.hpp"
#include "Module.hpp"
#include <oneapi/tbb/task_group.h>

EventLoopController::EventLoopController(EventSource& eventSource, int loopID)
    : eventSource_(eventSource), event_{loopID} {}
void EventLoopController::runAsync(tbb::task_group& tg) { processEventAsync(tg); }

void EventLoopController::addModule(Module* module) { modules_.push_back(module); }

void EventLoopController::processEventAsync(tbb::task_group& tg) {
  tg.run([this, &tg]() {
    auto nextEventOpt = eventSource_.nextEvent();
    if (!nextEventOpt.has_value()) {
      return;  // No more events to process
    }
    event_.clear();
    event_.setId(nextEventOpt.value());
    processNextModulesAsync(tg, 0);
  });
}
void EventLoopController::processNextModulesAsync(tbb::task_group& tg, int index) {
  if (index >= modules_.size()) {
    processEventAsync(tg);
    return;
  }
  Module* module = modules_[index];
  tg.run([this, &tg, module, index]() {
    module->processEvent(event_);
    processNextModulesAsync(tg, index + 1);
  });
}
