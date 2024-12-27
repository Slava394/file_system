#include <iostream>
#include <sstream>
#include "file_system.h"


FileSystem::FileSystem() 
{
    root = new Directory("/");
    current = root;
}

FileSystem::~FileSystem() 
{
    delete root;
}

void FileSystem::createFile(const std::string& name, const std::string& readPassword, const std::string& writePassword) 
{
    current->createFile(name, readPassword, writePassword);
}

void FileSystem::createDirectory(const std::string& name) 
{
    current->createSubdirectory(name);
}

void FileSystem::changeDirectory(const std::string& path) 
{
    if (path == "/") 
    {
        current = root;
        return;
    }

    if (path == "..") 
    {
        if (current->getParent() != nullptr) 
        {
            current = current->getParent();
        }
        return;
    }

    Directory* nextDir = current->findSubdirectory(path);
    if (nextDir == nullptr) 
    {
        throw std::runtime_error("Catalog not found");
    }
    current = nextDir;
}

void FileSystem::listDirectory() const 
{
    auto contents = current->listContents();
    for (const auto& item : contents) 
    {
        std::cout << item << std::endl;
    }
}

std::string FileSystem::getCurrentPath() const 
{
    std::string path = "";
    Directory* dir = current;

    while (dir != nullptr) 
    {
        path = "/" + dir->getName() + path;
        dir = dir->getParent();
    }

    return path.empty() ? "/" : path;
}