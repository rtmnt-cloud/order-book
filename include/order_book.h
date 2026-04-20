#pragma once

#include <unordered_map>
#include "order.h"
#include <optional>
#include "memory_pool.h"

static constexpr uint32_t MAX_PRICE_LEVELS = 1024;

struct PriceLevel {
    uint32_t head = NULL_INDEX;
    uint32_t tail = NULL_INDEX;
};

class OrderBook
{

    public:
    void add_order(Order order);
    bool cancel_order(uint64_t orderId);
    std::optional<Order> get_best_bid();
    std::optional<Order> get_best_ask();
    void print_book() const;

    private:
    // OLD (heap-allocated):
    // std::map<double, std::list<Order>> bids;
    // std::map<double, std::list<Order>> asks;
    // std::unordered_map<uint64_t, std::list<Order>::iterator> orderMap;

    PriceLevel bids[MAX_PRICE_LEVELS];
    PriceLevel asks[MAX_PRICE_LEVELS];
    std::unordered_map<uint64_t, uint32_t> orderMap;  // orderId -> pool index
    MemoryPool pool;
    uint32_t bestBid = 0;
    uint32_t bestAsk = MAX_PRICE_LEVELS - 1;

};