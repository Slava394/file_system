#include <iostream>
#include "fs.h"


int main() 
{
    PoolAllocator allocator; 
    FileSystem fs(&allocator); 

    fs.createFile("file1.txt", FileType::FILE);
    fs.createFile("dir1", FileType::DIRECTORY);

    auto inode = fs.openFile("file1.txt");
    if (inode) 
    {
        std::cout << "File opened successfully." << std::endl;
    } 
    else 
    {
        std::cout << "File not found." << std::endl;
    }

    fs.listFiles();

    if (fs.deleteFile("file1.txt")) 
    {
        std::cout << "File deleted successfully." << std::endl;
    } 
    else 
    {
        std::cout << "Failed to delete file." << std::endl;
    }

    fs.listFiles();

    return 0;
}
