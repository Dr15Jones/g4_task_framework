#include "EventProcessor.hpp"

int main(int argc, char *argv[])
{
    int numEvents = 100;
    int numConcurrentLoops = 4;
    int nThreads = 4;

    EventProcessor processor(numEvents, numConcurrentLoops, nThreads);

    // Add modules to the processor
    // ModuleA moduleA;
    // processor.addModuleToAllLoops(&moduleA);
    processor.run();

    return 0;
}