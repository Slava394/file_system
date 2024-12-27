#include "directory.h"
#include <stdexcept>
#include <iostream>


Directory::Directory(const std::string& name, Directory* parent)
    : name(name), parent(parent) {}

Directory::~Directory() 
{
    for (auto& [_, inode] : files) 
    {
        delete inode;
    }

    for (auto& [_, dir] : subdirectories) 
    {
        delete dir;
    }
}

const std::string& Directory::getName() const 
{
    return name;
}

Directory* Directory::getParent() const 
{
    return parent;
}

void Directory::createFile(const std::string& name, const std::string& readPassword, const std::string& writePassword) 
{
    if (files.find(name) != files.end() || subdirectories.find(name) != subdirectories.end()) 
    {
        throw std::runtime_error("Object already exist");
    }

    Inode* newFile = new Inode(FileType::FILE);
    newFile->setReadPassword(readPassword);
    newFile->setWritePassword(writePassword);
    files[name] = newFile;
}

Directory* Directory::createSubdirectory(const std::string& name) 
{
    if (files.find(name) != files.end() || subdirectories.find(name) != subdirectories.end()) 
    {
        throw std::runtime_error("Object already exist");
    }

    Directory* newDir = new Directory(name, this);
    subdirectories[name] = newDir;
    return newDir;
}

Inode* Directory::findFile(const std::string& name) const 
{
    auto it = files.find(name);
    if (it != files.end()) 
    {
        return it->second;
    }
    return nullptr;
}

Directory* Directory::findSubdirectory(const std::string& name) const 
{
    auto it = subdirectories.find(name);
    if (it != subdirectories.end()) 
    {
        return it->second;
    }
    return nullptr;
}

void Directory::deleteFile(const std::string& name) 
{
    auto it = files.find(name);
    if (it == files.end()) 
    {
        throw std::runtime_error("File not found");
    }
    delete it->second; 
    files.erase(it);   
}

void Directory::deleteSubdirectory(const std::string& name) 
{
    auto it = subdirectories.find(name);
    if (it == subdirectories.end()) 
    {
        throw std::runtime_error("Catalog not found");
    }
    delete it->second; 
    subdirectories.erase(it); 
}

std::vector<std::string> Directory::listContents() const 
{
    std::vector<std::string> contents;

    for (const auto& [name, _] : files) 
    {
        contents.push_back("[FILE] " + name);
    }

    for (const auto& [name, _] : subdirectories) 
    {
        contents.push_back("[DIR] " + name);
    }

    return contents;
}
