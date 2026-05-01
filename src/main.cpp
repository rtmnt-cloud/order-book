#include <iostream>
#include <thread>
#include <atomic>
#include "simulator.h"

int main() {
    
    Simulator simulator;

    simulator.run(10000);
    simulator.printStats();

    return 0;
}
