#include <iostream>
#include <thread>
#include <atomic>
#include "matching_engine.h"
#include "spsc_queue.h"

int main() {
    SPSCQueue queue;
    MatchingEngine engine;

    const int NUM_ORDERS = 10000;
    std::atomic<bool> done{false};

    // producer: generate orders and push into queue
    std::thread producer([&]() {
        srand(42);
        for (int i = 0; i < NUM_ORDERS; i++) {
            Order o;
            o.orderId      = i;
            o.traderId     = rand() % 1000;
            o.instrumentId = 1001;
            o.side         = (i % 2 == 0) ? Side::Buy : Side::Sell;
            o.price        = rand() % 100;
            o.quantity     = rand() % 100 + 1;
            o.timestamp    = std::chrono::steady_clock::now();

            while (!queue.push(o));  // spin until space available
        }
        done.store(true);
    });

    // consumer: pop orders and process them
    std::thread consumer([&]() {
        Order o;
        while (!done.load() || !queue.empty()) {
            if (queue.pop(o))
                engine.process_order(o);
        }
    });

    producer.join();
    consumer.join();

    const auto& trades = engine.get_trades();
    std::cout << "Total orders : " << NUM_ORDERS << "\n";
    std::cout << "Total trades : " << trades.size() << "\n";

    return 0;
}
