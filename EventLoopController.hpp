#ifndef EVENTLOOPCONTROLLER_HPP
#define EVENTLOOPCONTROLLER_HPP

#include <oneapi/tbb/task_group.h>
#include "Event.hpp"
#include <vector>
class EventSource;
class Module;

class EventLoopController {
public:
  EventLoopController(EventSource& eventSource, int loopID);
  void runAsync(tbb::task_group& tg);

  void addModule(Module* module);

private:
  void processEventAsync(tbb::task_group& tg);
  void processNextModulesAsync(tbb::task_group& tg, int index);
  EventSource& eventSource_;
  Event event_;
  std::vector<Module*> modules_;
};
#endif
