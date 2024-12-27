#pragma once
#include <cstddef>
#include <stdexcept>
#include <bitset>
#define BLOCK_SIZE 4096
#define POOL_SIZE 261632 // (near 1 gb) 1073737728 b / 4kb

class PoolAllocator 
{
private:
    void* poolMemory;            
    std::bitset<POOL_SIZE> freeBlocks;            
    size_t totalBlocks;          

public:
    PoolAllocator();
    ~PoolAllocator();

    void* allocate();

    void deallocate(void* block);
};