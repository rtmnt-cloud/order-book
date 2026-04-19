#pragma once
#include <vector>
#include "order.h"

static constexpr uint32_t POOL_SIZE = 65536;

class MemoryPool
{
    public:
        MemoryPool();

        Order* allocate();
        void free(uint32_t index);

        Order slots[POOL_SIZE];  // public so OrderBook can index directly

    private:
        std::vector<uint32_t> freeList;
};



