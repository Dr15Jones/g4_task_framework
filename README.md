# Introduction

This is a trivial framework that behaves similar to the CMSSW framework where a asynchronous tasks are run via Intel's Thread Building Block library. Once one task finishes, it queues a new task to be run. Once all Events have been processed, no further tasks are queued and the job will then finish.

# Module types
For the framework a `Module` encapsulates some task that needs to be accomplished.

## AsyncModule
This is the base class for all module types. If inherited directly one must implemebnt the function
```
void processEventAsync(tbb::task_group& iGroup, tbb::task_handle iHandle, Event& iEvent) const
```
The actual work to be done must be started asynchronously by calling `iGroup.run`. Once the work has completed one must call `iGroup.run(std::move(iHandle))` in order for the next waiting task to be started.
If an exception is thrown in the functor passed to `iGroup.run` the task processing will stop and the exception will be printed.

This interface allows one to launch other asynchronous sub-tasks (either with TBB or using other facilities) and then when all those sub-task have been completed one just needs to call `iGroup.run(std::move(iHandle))` to have the framework proceed with other scheduled work.


## ReentrantModule
This class inherits from `AsyncModule` and provides the following interface
```
void processEvent(Event& iEvent) const
```
This method can be called simultaneously from multiple event loops and therefore must be thread-safe. The `ReentrantModule` handles the details related to the `tbb::task_handle`.

## LoopCloneModule<T>
This class inherits from `AsyncModule`. Internally, the module makes N copies of the class `T` where N is required to be the number of concurrent event loops.
The class `T` must have the following interface
```
void processEvent(Event& iEvent);
```
The method does not have to be const. Each instance of T is guaranteed to never have multiple threads call its `processEvent` method concurrently, therefore internal state of the `T` instance is allowed to change during the call.

# How to Extend
In the `main.cc` file you should instantiate classes that inherit directly or indirectly from AsyncModule and pass their address to `EventProcessor::addModuleToAllLoops`. Then call `EventProcessor::run`. Feel free to change the parameters passed to `EventProcessor`'s constructor but it is best if number of event loops is equal to or less than the number of threads.

Note that any modules passed to `EventProcessor::addModuleToAllLoops` will be shared across all concurrently running loops and therefore the module's `processEvent` method must be thread safe. One easy way to do that is to pass to the module the number of concurent loops being used in the program and internally create a separate mutable state for each loop. One can get access to the loop ID by calling `Event::loopID` which can then be used to pick the proper mutable state. The framework guarantees that each Module will never have a task associated with the same loop ID run concurrently.