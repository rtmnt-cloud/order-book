#include "memory_pool.h"


MemoryPool::MemoryPool()
{
    freeList.reserve(POOL_SIZE);
    for (uint32_t i = 0; i < POOL_SIZE; i++)
        freeList.push_back(i);
}
void MemoryPool::free(uint32_t index)
{

    freeList.push_back(index);

}
Order* MemoryPool:: allocate()
{
    if(freeList.empty())
    return nullptr;
    
    uint32_t slot = freeList.back();
    freeList.pop_back();
    return &slots[slot];


}