#include <iostream>
#include "matching_engine.h"

int main() {
    MatchingEngine engine;
    // uint64_t orderId;
    // uint64_t traderId;
    // uint64_t instrumentId;
    // Side side;
    // double price;   
    // uint32_t  quantity;
    // std::chrono::steady_clock::time_point timestamp;
    std::vector<Order> orders = {
        {1,  101, 1001, Side::Buy,  100.00, 50,  std::chrono::steady_clock::now()},
        {2,  102, 1001, Side::Buy,   99.00, 30,  std::chrono::steady_clock::now()},
        {3,  103, 1001, Side::Buy,   98.00, 100, std::chrono::steady_clock::now()},
        {4,  104, 1001, Side::Sell,  99.00, 40,  std::chrono::steady_clock::now()},
        // {5,  105, 1001, Side::Sell,  98.50, 20,  std::chrono::steady_clock::now()},
        // {6,  106, 1001, Side::Buy,  101.00, 200, std::chrono::steady_clock::now()},
        // {7,  107, 1001, Side::Sell, 100.00, 150, std::chrono::steady_clock::now()},
        // {8,  108, 1001, Side::Sell,  97.00, 10,  std::chrono::steady_clock::now()},
        // {9,  109, 1001, Side::Buy,   95.00, 500, std::chrono::steady_clock::now()},
        // {10, 110, 1001, Side::Sell, 200.00, 100, std::chrono::steady_clock::now()},
    };

    for (auto& o : orders)
        engine.process_order(o);

    const auto& trades = engine.get_trades();
    std::cout << "Total trades: " << trades.size() << "\n";
    for (const auto& t : trades) {
        std::cout << "  buyer=" << t.buyerId
                  << " seller=" << t.sellerId
                  << " price=" << t.price
                  << " qty=" << t.quantity
                  << "\n";
    }

    engine.print_book();

    return 0;
}
