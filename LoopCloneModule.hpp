#ifndef LOOPCLONEMODULE_HPP
#define LOOPCLONEMODULE_HPP

#include "AsyncModule.hpp"
#include "Event.hpp"
#include <vector>
#include <stdexcept>

/// @brief A module that clones a given module type for each event loop and delegates processing to the appropriate clone based on the event's loop ID.
/// @tparam T The type of the module to be cloned. Must have the function `void processEvent(Event& event)`.

template <typename T>
class LoopCloneModule : public AsyncModule {
public:
  template <typename... Args>
  LoopCloneModule(int nConcurrentEventLoops, Args... args) : modules_(), handles_(nConcurrentEventLoops) {
    modules_.reserve(nConcurrentEventLoops);
    for (int i = 0; i < nConcurrentEventLoops; ++i) {
      modules_.emplace_back(args...);
    }
  }
  void processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& iEvent) const final {
    int loopID = iEvent.loopID();
    if (loopID < 0 || loopID >= static_cast<int>(modules_.size())) {
      throw std::out_of_range("Invalid loop ID in LoopCloneModule");
    }
    auto& clone = modules_[loopID];
    handles_[loopID] = std::move(iHandle);
    auto& handle = handles_[loopID];
    iGroup.run([&iGroup, &handle, &iEvent, &clone]() {
      auto localHandle = std::move(handle);
      clone.processEvent(iEvent);
      iGroup.run(std::move(localHandle));
    });
  }

private:
  mutable std::vector<T> modules_;
  mutable std::vector<tbb::task_handle> handles_;
};
#endif
