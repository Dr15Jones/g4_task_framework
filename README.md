# Introduction

This is a trivial framework that behaves similar to the CMSSW framework where a asynchronous tasks are run via Intel's Thread Building Block library. Once one task finishes, it queues a new task to be run. Once all Events have been processed, no further tasks are queued and the job will then finish.

# How to Extend
In the `main.cc` file you should instantiate classes that inherit from Module and pass their address to `EventProcessor::addModuleToAllLoops`. Then call `EventProcessor::run`. Feel free to change the parameters passed to `EventProcessor`'s constructor but it is best if number of event loops is equal to or less than the number of threads.

Note that any modules passed to `EventProcessor::addModuleToAllLoops` will be shared across all concurrently running loops and therefore the module's `processEvent` method must be thread safe. One easy way to do that is to pass to the module the number of concurent loops being used in the program and internally create a separate mutable state for each loop. One can get access to the loop ID by calling `Event::loopID` which can then be used to pick the proper mutable state. The framework guarantees that each Module will never have a task associated with the same loop ID run concurrently.