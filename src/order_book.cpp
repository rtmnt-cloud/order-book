#include "order_book.h"
#include <iostream>

// ===================== OLD VERSION (std::list, heap-allocated) =====================

// void OrderBook::add_order(Order order)
// {
//     auto& book = (order.side == Side::Buy) ? bids : asks;
//     auto& list = book[order.price];
//     auto it = list.insert(list.end(), order);
//     orderMap[order.orderId] = it;
// }

// bool OrderBook::cancel_order(uint64_t orderId)
// {
//     auto it = orderMap.find(orderId);
//     if (it == orderMap.end())
//         return false;
//     double price = it->price;
//     auto& book = (it->side == Side::Buy) ? bids : asks;
//     auto& list = book[price];
//     list.erase(it);
//     if (list.empty())
//         book.erase(price);
//     orderMap.erase(orderId);
//     return true;
// }

// std::optional<Order> OrderBook::get_best_bid()
// {
//     if (bids.empty()) return std::nullopt;
//     return bids.rbegin()->second.front();
// }

// std::optional<Order> OrderBook::get_best_ask()
// {
//     if (asks.empty()) return std::nullopt;
//     return asks.begin()->second.front();
// }

// ===================== NEW VERSION (pool-backed, zero heap alloc) =====================

void OrderBook::add_order(Order order)
{
    auto& book = (order.side == Side::Buy) ? bids : asks;

    Order* node = pool.allocate();
    if (!node) return;
    uint32_t nodeIdx = node - pool.slots;
    *node = order;
    node->next = NULL_INDEX;

auto& level = book[static_cast<uint32_t>(order.price)];
    if (level.head == NULL_INDEX) {
        node->prev = NULL_INDEX;
        level.head = level.tail = nodeIdx;
    } else {
        node->prev = level.tail;
        pool.slots[level.tail].next = nodeIdx;
        level.tail = nodeIdx;
    }
    orderMap[order.orderId] = nodeIdx;

    uint32_t priceIdx = static_cast<uint32_t>(order.price);
    if (order.side == Side::Buy && priceIdx > bestBid)
        bestBid = priceIdx;
    if (order.side == Side::Sell && priceIdx < bestAsk)
        bestAsk = priceIdx;
}

bool OrderBook::cancel_order(uint64_t orderId)
{
    auto it = orderMap.find(orderId);
    if (it == orderMap.end())
        return false;

    uint32_t idx = it->second;
    Order& order = pool.slots[idx];
    auto& book = (order.side == Side::Buy) ? bids : asks;
    uint32_t priceIdx = static_cast<uint32_t>(order.price);
    auto& level = book[priceIdx];

    if (order.prev != NULL_INDEX)
        pool.slots[order.prev].next = order.next;
    else
        level.head = order.next;

    if (order.next != NULL_INDEX)
        pool.slots[order.next].prev = order.prev;
    else
        level.tail = order.prev;

    if (level.head == NULL_INDEX) {
        level = PriceLevel{};
        // update best tracker if this level was the best
        if (order.side == Side::Buy && priceIdx == bestBid) {
            while (bestBid > 0 && bids[bestBid].head == NULL_INDEX) bestBid--;
        }
        if (order.side == Side::Sell && priceIdx == bestAsk) {
            while (bestAsk < MAX_PRICE_LEVELS - 1 && asks[bestAsk].head == NULL_INDEX) bestAsk++;
        }
    }

    pool.free(idx);
    orderMap.erase(it);
    return true;
}

std::optional<Order> OrderBook::get_best_bid()
{
    if (bids[bestBid].head == NULL_INDEX) return std::nullopt;
    return pool.slots[bids[bestBid].head];
}

std::optional<Order> OrderBook::get_best_ask()
{
    if (asks[bestAsk].head == NULL_INDEX) return std::nullopt;
    return pool.slots[asks[bestAsk].head];
}

void OrderBook::print_book() const
{
    std::cout << "=== BIDS ===\n";
    for (int i = MAX_PRICE_LEVELS - 1; i >= 0; i--) {
        if (bids[i].head == NULL_INDEX) continue;
        uint32_t idx = bids[i].head;
        uint32_t count = 0, total = 0;
        while (idx != NULL_INDEX) {
            const Order& o = pool.slots[idx];
            total += o.quantity;
            idx = o.next;
            count++;
        }
        std::cout << "  price=" << i << " qty=" << total << " orders=" << count << "\n";
    }

    std::cout << "=== ASKS ===\n";
    for (uint32_t i = 0; i < MAX_PRICE_LEVELS; i++) {
        if (asks[i].head == NULL_INDEX) continue;
        uint32_t idx = asks[i].head;
        uint32_t count = 0, total = 0;
        while (idx != NULL_INDEX) {
            const Order& o = pool.slots[idx];
            total += o.quantity;
            idx = o.next;
            count++;
        }
        std::cout << "  price=" << i << " qty=" << total << " orders=" << count << "\n";
    }
}
