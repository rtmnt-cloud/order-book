#pragma once

#include <cstdint>
#include <chrono>

struct Trade
{
uint64_t buyer_id;
uint64_t seller_id;
uint64_t instrument_id;
double price;
uint32_t quantity;
std::chrono::steady_clock::time_point timestamp;
};