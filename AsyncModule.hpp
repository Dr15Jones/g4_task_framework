#ifndef ASYNCMODULE_HPP
#define ASYNCMODULE_HPP

#include <oneapi/tbb/task_group.h>
#include "Event.hpp"

class AsyncModule {
public:
  virtual ~AsyncModule() = default;
  /// @brief asynchronously processes the event. Must call iGroup.run with the handle to continue processing.
  /// @param  iGroup  the asynchronous work should be scheduled by calling iGroup.run and should be finished by calling the iGroup.run(iHandle).
  /// @param  iHandle the handle to be called to continue processing the event.
  /// @param iEvent the event to be processed. 
  virtual void processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& iEvent) const = 0;
};

#endif
