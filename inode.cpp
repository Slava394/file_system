#include "inode.h"


DiskBlock::DiskBlock() : next(nullptr) 
{
    std::fill(std::begin(data), std::end(data), 0);
}


Inode::~Inode() 
{
    releaseBlocks(); 
}

void Inode::releaseBlocks() 
{
    DiskBlock* current = firstBlock;
    while (current) 
    {
        DiskBlock* next = current->next;
        allocator->deallocate(current);
        current = next;
    }
    firstBlock = nullptr;
    fileSize = 0;
    modificationTime = std::time(nullptr);
}

Inode::Inode(PoolAllocator* poolAllocator, FileType type)
    : firstBlock(nullptr), fileSize(0),
      creationTime(std::time(nullptr)),
      modificationTime(std::time(nullptr)),
      fileType(type),
      allocator(poolAllocator) {}

DiskBlock* Inode::getFirstBlock() const 
{
    return firstBlock;
}

size_t Inode::getFileSize() const 
{
    return fileSize;
}

void Inode::setReadPassword(const std::string& password)
{
    readPassword = password;
}

void Inode::setWritePassword(const std::string& password) 
{
    writePassword = password;
}

bool Inode::checkReadPassword(const std::string& password) const 
{
    return readPassword.empty() || password == readPassword;
}

bool Inode::checkWritePassword(const std::string& password) const 
{
    return writePassword.empty() || password == writePassword;
}

void Inode::writeData(const char* data, size_t size, const std::string& password) 
{
    if (!checkWritePassword(password))
    {
        throw std::runtime_error("Access denied");
    }

    releaseBlocks(); 

    size_t bytesWritten = 0;
    DiskBlock* currentBlock = nullptr;

    while (bytesWritten < size) 
    {
        DiskBlock* newBlock = static_cast<DiskBlock*>(allocator->allocate());
        if (!newBlock) 
        {
            throw std::runtime_error("Failed to allocate memory");
        }

        size_t bytesToCopy = std::min(size - bytesWritten, static_cast<size_t>(BLOCK_SIZE));
        std::copy(data + bytesWritten, data + bytesWritten + bytesToCopy, newBlock->data);

        if (currentBlock) 
        {
            currentBlock->next = newBlock;
        } 
        else 
        {
            firstBlock = newBlock;
        }

        currentBlock = newBlock;
        currentBlock->next = nullptr;
        bytesWritten += bytesToCopy;
    }

    fileSize = size;
    modificationTime = std::time(nullptr);
}

size_t Inode::readData(char* buffer, size_t bufferSize, const std::string& password) const 
{
    if (!checkWritePassword(password))
    {
        throw std::runtime_error("Acess denied");
    }

    if (!firstBlock) 
    {
        return 0; 
    }

    size_t bytesRead = 0;
    DiskBlock* currentBlock = firstBlock;

    while (currentBlock && bytesRead < bufferSize) 
    {
        size_t bytesToCopy = std::min(bufferSize - bytesRead, static_cast<size_t>(BLOCK_SIZE));
        std::copy(currentBlock->data, currentBlock->data + bytesToCopy, buffer + bytesRead);

        bytesRead += bytesToCopy;
        currentBlock = currentBlock->next;
    }

    return bytesRead;
}

std::time_t Inode::getCreationTime() const 
{
    return creationTime;
}

std::time_t Inode::getModificationTime() const 
{
    return modificationTime;
}

FileType Inode::getFileType() const 
{
    return fileType;
}

void Inode::setFileType(FileType type) 
{
    fileType = type;
}