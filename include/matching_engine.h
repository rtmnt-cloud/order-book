#pragma once
#include <cstdint>
#include <vector>
#include "order_book.h"
#include "trade.h"

class MatchingEngine
{
public:
    void process_order(Order order);
    const std::vector<Trade>& get_trades() const;

private:
    OrderBook orderBook;
    std::vector<Trade> trades;
};
