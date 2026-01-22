#include "ReentrantModule.hpp"
#include <memory>

void ReentrantModule::processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& iEvent) const {
  auto handlePtr = std::make_unique<tbb::task_handle>(std::move(iHandle));
  iGroup.run([this, &iEvent, group = &iGroup, handlePtr = std::move(handlePtr)]() {
    // Synchronously process the event
    processEvent(iEvent);
    // Continue processing by calling the handle
    group->run(std::move(*handlePtr));
  });
}
