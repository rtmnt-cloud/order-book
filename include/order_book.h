#pragma once

#include <map>
#include <list>
#include <unordered_map>
#include "order.h"
#include <optional>

class OrderBook
{

    public:
    void add_order(const Order& order);
    bool cancel_order(uint64_t orderId);
    std::optional<Order> get_best_bid();
    std::optional<Order> get_best_ask(); 



    private:
    std::map<double, std::list<Order>> bids;
    std::map<double, std::list<Order>> asks;
    std::unordered_map<uint64_t,
    std::list<Order>::iterator> orderMap;




};