#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "pool_allocator.h"


PoolAllocator::PoolAllocator() 
{
    poolMemory = std::malloc(POOL_SIZE * (BLOCK_SIZE + 8));  
    if (!poolMemory) {
        throw std::runtime_error("Memory exception");
    }

    totalBlocks = POOL_SIZE;
    freeBlocks.reset();
}

PoolAllocator::~PoolAllocator() 
{
    std::free(poolMemory);  
}

void* PoolAllocator::allocate() 
{
    for (size_t i = 0; i < totalBlocks; ++i) 
    {
        if (!freeBlocks.test(i)) 
        {
            freeBlocks.set(i);  
            return static_cast<char*>(poolMemory) + i * (BLOCK_SIZE + 8);  
        }
    }
    throw std::runtime_error("There are no free blocks");
}

void PoolAllocator::deallocate(void* block) 
{
    size_t index = (static_cast<char*>(block) - static_cast<char*>(poolMemory)) / (BLOCK_SIZE + 8);
    if (index >= totalBlocks || freeBlocks[index]) 
    {
        throw std::invalid_argument("Something went wrong");
    }
    freeBlocks.reset(index);  
}