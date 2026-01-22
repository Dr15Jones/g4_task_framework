#ifndef EVENTPROCESSOR_HPP
#define EVENTPROCESSOR_HPP

#include <vector>
#include "EventLoopController.hpp"
#include "EventSource.hpp"

class AsyncModule;

class EventProcessor {
public:
  EventProcessor(int nEvents, int nConcurrentLoops, int nThreads);
  void addModuleToAllLoops(AsyncModule* module);
  void run();

private:
  EventSource eventSource_;
  std::vector<EventLoopController> controllers_;
  int nThreads_;
};

#endif
