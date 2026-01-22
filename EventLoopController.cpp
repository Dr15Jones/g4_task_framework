
#include "EventLoopController.hpp"
#include "EventSource.hpp"
#include "AsyncModule.hpp"
#include <oneapi/tbb/task_group.h>

EventLoopController::EventLoopController(EventSource& eventSource, int loopID)
    : eventSource_(eventSource), event_{loopID} {}
void EventLoopController::runAsync(tbb::task_group& tg) { processEventAsync(tg); }

void EventLoopController::addModule(AsyncModule* module) { modules_.push_back(module); }

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
  auto* module = modules_[index];
  auto handle = tg.defer([this, &tg, index]() {
    processNextModulesAsync(tg, index + 1);
  });
  module->processEventAsync(tg, std::move(handle), event_);
}
