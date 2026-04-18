#pragma once

#include <cstdint>
#include <chrono>

struct Trade
{
uint64_t buyerId;
uint64_t sellerId;
uint64_t instrumentId;
double price;
uint32_t quantity;
std::chrono::steady_clock::time_point timestamp;
};