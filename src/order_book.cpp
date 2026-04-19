#include "order_book.h"
#include <iostream>
// You know the steps:

// Check the order's side
// Pick the right map (bids or asks)
// Add the order to the list at that price level
// Store the iterator in the lookup map

void OrderBook::add_order(const Order& order)
{
    auto& book = (order.side == Side::Buy) ? bids : asks;
    auto& list = book[order.price];
    auto it = list.insert(list.end(), order);
    orderMap[order.orderId] = it;

}

    // Find the order ID in the lookup map
    // If not found, return false
    // If found, you have an iterator to the list node. 
    // But which map (bids or asks) and which price level?
    //  You need to figure out how to get there.
bool OrderBook::cancel_order(uint64_t orderId)
{
    auto mapIt = orderMap.find(orderId);
    if (mapIt == orderMap.end())
    return false;
    
    auto it = mapIt->second;
    double price = it->price;
    auto& book = (it->side == Side::Buy)? bids:asks;
    auto& list = book[price];
    list.erase(it);
    if (list.empty())
    book.erase(price);
    orderMap.erase(orderId);
    return true;

}

std::optional<Order> OrderBook:: get_best_bid()
{
    if(bids.empty())
    return std::nullopt;
    else
    return bids.rbegin()->second.front();
}
std::optional<Order> OrderBook::get_best_ask()
{
    if(asks.empty())
    return std::nullopt;
    else
    return asks.begin()->second.front();
}

    // private:
    // std::map<double, std::list<Order>> bids;
    // std::map<double, std::list<Order>> asks;
    // std::unordered_map<uint64_t,
    // std::list<Order>::iterator> orderMap;

void OrderBook::print_book() const
{
    std::cout << "=== BIDS ===\n";
    for (auto it = bids.rbegin(); it != bids.rend(); ++it) {
        uint32_t total = 0;
        for (const auto& o : it->second) total += o.quantity;
        std::cout << "  price=" << it->first << " qty=" << total << " orders=" << it->second.size() << "\n";
    }

    std::cout << "=== ASKS ===\n";
    for (const auto& [price, list] : asks) {
        uint32_t total = 0;
        for (const auto& o : list) total += o.quantity;
        std::cout << "  price=" << price << " qty=" << total << " orders=" << list.size() << "\n";
    }
}