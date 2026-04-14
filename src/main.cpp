#include <iostream>
#include "order_book.h"

int main() {
    OrderBook book;

    book.add_order({1, Side::Buy, 50.00, 100, std::chrono::steady_clock::now()});
    book.add_order({2, Side::Buy, 51.00, 200, std::chrono::steady_clock::now()});
    book.add_order({3, Side::Sell, 52.00, 150, std::chrono::steady_clock::now()});

    auto bestBid = book.get_best_bid();
    auto bestAsk = book.get_best_ask();

    if (bestBid)
        std::cout << "Best bid: " << bestBid->price << std::endl;
    if (bestAsk)
        std::cout << "Best ask: " << bestAsk->price << std::endl;

    book.cancel_order(2);
    bestBid = book.get_best_bid();
    if (bestBid)
        std::cout << "Best bid after cancel: " << bestBid->price << std::endl;

    return 0;
}
