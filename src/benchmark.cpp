#include <iostream>
#include "matching_engine.h"

int main() {
    MatchingEngine engine;
    srand(42);

    auto start = std::chrono::steady_clock::now();
    for( int i = 0 ; i < 1000000; i ++)
    {
        uint64_t traderId = rand()%1000;
        uint64_t instrumentId = rand()%10;
        Side side = i % 2 == 0 ? Side::Buy : Side::Sell;
        double price = rand() % 100;
        uint32_t qty =  rand()% 100;
        auto timestamp = std::chrono::steady_clock::now();
        engine.process_order(Order{i, traderId,instrumentId, side, price, qty,timestamp}); 

    }


    auto end = std::chrono::steady_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Total time  : " << ns << " ns\n";
    std::cout << "Avg latency : " << ns / 1000000 << " ns/order\n";
    std::cout << "Throughput  : " << (1000000000LL * 1000000) / ns << " orders/sec\n";

    return 0;
}
