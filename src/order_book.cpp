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
    Order* slot = pool.allocate();
    if (!slot) return;
    uint32_t idx = slot - pool.slots;
    *slot = order;
    slot->next = NULL_INDEX;

    auto& level = book[order.price];
    if (level.head == NULL_INDEX) {
        slot->prev = NULL_INDEX;
        level.head = level.tail = idx;
    } else {
        slot->prev = level.tail;
        pool.slots[level.tail].next = idx;
        level.tail = idx;
    }
    orderMap[order.orderId] = idx;
}

bool OrderBook::cancel_order(uint64_t orderId)
{
    auto it = orderMap.find(orderId);
    if (it == orderMap.end())
        return false;

    uint32_t idx = it->second;
    Order& order = pool.slots[idx];
    auto& book = (order.side == Side::Buy) ? bids : asks;
    auto& level = book[order.price];

    if (order.prev != NULL_INDEX)
        pool.slots[order.prev].next = order.next;
    else
        level.head = order.next;

    if (order.next != NULL_INDEX)
        pool.slots[order.next].prev = order.prev;
    else
        level.tail = order.prev;

    if (level.head == NULL_INDEX)
        book.erase(order.price);

    pool.free(idx);
    orderMap.erase(it);
    return true;
}

std::optional<Order> OrderBook::get_best_bid()
{

    if (bids.empty()) return std::nullopt;
    return pool.slots[bids.rbegin()->second.head];

}

std::optional<Order> OrderBook::get_best_ask()
{
    if (asks.empty()) return std::nullopt;
    return pool.slots[asks.begin()->second.head];
}

void OrderBook::print_book() const
{
    std::cout << "=== BIDS ===\n";
    for (auto it = bids.rbegin(); it != bids.rend(); ++it) {
        uint32_t idx = it->second.head;
        uint32_t count = 0, total = 0;
        while (idx != NULL_INDEX) {
            const Order& o = pool.slots[idx];
            total += o.quantity;
            idx = o.next;
            count++;
        }
        std::cout << "  price=" << it->first << " qty=" << total << " orders=" << count << "\n";
    }

    std::cout << "=== ASKS ===\n";
    for (const auto& [price, level] : asks) {
        uint32_t idx = level.head;
        uint32_t count = 0, total = 0;
        while (idx != NULL_INDEX) {
            const Order& o = pool.slots[idx];
            total += o.quantity;
            idx = o.next;
            count++;
        }
        std::cout << "  price=" << price << " qty=" << total << " orders=" << count << "\n";
    }
}
