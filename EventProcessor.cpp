#include "EventProcessor.hpp"
#include <iostream>
#include <oneapi/tbb/task_group.h>
#include <oneapi/tbb/task_arena.h>
EventProcessor::EventProcessor(int nEvents, int nConcurrentLoops, int nThreads)
    : eventSource_(nEvents), nThreads_(nThreads) {
  controllers_.reserve(nConcurrentLoops);
  for (int i = 0; i < nConcurrentLoops; ++i) {
    controllers_.emplace_back(eventSource_, i);
  }
}
void EventProcessor::addModuleToAllLoops(Module* module) {
  for (auto& controller : controllers_) {
    controller.addModule(module);
  }
}
void EventProcessor::run() {
  tbb::task_arena arena(nThreads_);
  arena.execute([&]() {
    tbb::task_group tg;

    for (auto& controller : controllers_) {
      controller.runAsync(tg);
    }
    tg.wait();
  });
}
