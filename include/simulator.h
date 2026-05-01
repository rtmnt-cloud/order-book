#pragma once
#include "matching_engine.h"
#include "spsc_queue.h"




class Simulator
{

    MatchingEngine engine;
    int totalOrders;
    int totalTrades;
    double midPrice;
    SPSCQueue queue;

    public:

        Simulator();
        void run(int numOrders);
        void printStats() const;





};


