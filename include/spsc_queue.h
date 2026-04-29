#pragma once
#include <atomic>
#include "order.h"
#include <cstdint>

static constexpr uint32_t CAPACITY = 1024;
class SPSCQueue
{

    public:
    bool push(const Order&);
    bool pop(Order&);
    bool empty() const { return head.load() == tail.load(); }



    private:
        Order buffer[CAPACITY];
        std::atomic<uint32_t> head{0}, tail{0};


};