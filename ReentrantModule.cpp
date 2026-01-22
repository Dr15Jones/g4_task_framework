#include "ReentrantModule.hpp"

void ReentrantModule::processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& iEvent) const {
  iGroup.run([this, &iEvent, group = &iGroup, handle = std::move(iHandle)]() {
    // Synchronously process the event
    processEvent(iEvent);
    // Continue processing by calling the handle
    group->run(std::move(const_cast<tbb::task_handle&>(handle)));
  });
}
