#pragma once
#include <cstdint>
#include <chrono>

enum class Side {
    Buy,
    Sell
};

struct Order {
    uint64_t id;
    uint64_t trader_id;
    uint64_t instrument_id;
    Side side;
    double price;   
    uint32_t  quantity;
    std::chrono::steady_clock::time_point timestamp;
};