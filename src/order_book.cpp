#include "order_book.h"
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
    orderMap[order.id] = it;

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

// Otherwise the logic is correct. 
// Fix those two, save the file, and then write get_best_bid()
//  and get_best_ask(). 
// Those should be quick for you now.
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