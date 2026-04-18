#pragma once
#include <cstdint>
#include <chrono>

enum class Side {
    Buy,
    Sell
};

struct Order {
    uint64_t orderId;
    uint64_t traderId;
    uint64_t instrumentId;
    Side side;
    double price;   
    uint32_t  quantity;
    std::chrono::steady_clock::time_point timestamp;
};