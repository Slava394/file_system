#pragma once
#include <string>
#include <ctime>
#include "pool_allocator.h"
#define BLOCK_SIZE 4096


enum class FileType 
{
    FILE,
    DIRECTORY
};


struct DiskBlock 
{
    char data[BLOCK_SIZE];
    DiskBlock* next;      

    DiskBlock();
};


class Inode 
{
private:
    DiskBlock* firstBlock;     
    size_t fileSize;          
    std::time_t creationTime;  
    std::time_t modificationTime;
    std::string readPassword; 
    std::string writePassword; 
    FileType fileType; 
    PoolAllocator* allocator;

public:
    Inode(PoolAllocator* allocator, FileType type = FileType::FILE);
    ~Inode();

    DiskBlock* getFirstBlock() const;
    // void setFirstBlock(DiskBlock* block);

    void writeData(const char* data, size_t size, const std::string& password);
    size_t readData(char* buffer, size_t bufferSize, const std::string& password) const;
    void releaseBlocks();

    size_t getFileSize() const;
    // void setFileSize(size_t size);

    void setReadPassword(const std::string& password);
    void setWritePassword(const std::string& password);
    bool checkReadPassword(const std::string& password) const;
    bool checkWritePassword(const std::string& password) const;

    std::time_t getCreationTime() const;
    std::time_t getModificationTime() const;

    FileType getFileType() const;  
    void setFileType(FileType type);
};