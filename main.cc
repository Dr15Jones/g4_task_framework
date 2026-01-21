#include "Module.hpp"

#include "EventProcessor.hpp"

int main(int argc, char *argv[])
{
    int numEvents = 100;
    int numConcurrentLoops = 4;
    int nThreads = 4;

    EventProcessor processor(numEvents, numConcurrentLoops, nThreads);
    processor.run();

    return 0;
}