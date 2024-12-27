#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "inode.h"


class FileSystem 
{
private:
    std::unordered_map<std::string, std::shared_ptr<Inode>> fileTable; 
    PoolAllocator* allocator; 

public:
    FileSystem(PoolAllocator* poolAllocator)
        : allocator(poolAllocator) {}


    bool createFile(const std::string& filename, FileType type = FileType::FILE)
    {

        if (fileTable.find(filename) != fileTable.end()) 
        {
            return false; 
        }

        std::shared_ptr<Inode> inode = std::make_shared<Inode>(allocator, type);
        fileTable[filename] = inode;
        return true;
    }

    std::shared_ptr<Inode> openFile(const std::string& filename) 
    {
        auto it = fileTable.find(filename);
        if (it != fileTable.end()) 
        {
            return it->second; 
        }
        return nullptr; 
    }

    bool deleteFile(const std::string& filename) 
    {
        auto it = fileTable.find(filename);
        if (it != fileTable.end()) 
        {
            fileTable.erase(it); 
            return true;
        }
        return false;
    }

    void listFiles() const 
    {
        for (const auto& entry : fileTable) 
        {
            std::cout << "File: " << entry.first << std::endl;
        }
    }
};
