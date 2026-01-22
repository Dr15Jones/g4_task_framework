#ifndef REENTRANTMODULE_HPP
#define ReentrantModule_HPP

#include "AsyncModule.hpp"

/// @brief 
class ReentrantModule : public AsyncModule {
protected:
  virtual void processEvent(Event& event) const = 0;

private:
  void processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& iEvent) const final;
};
#endif