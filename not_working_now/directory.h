#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "inode.h"

class Directory 
{
private:
    std::string name;                              
    Directory* parent;                            
    std::unordered_map<std::string, Inode*> files; 
    std::unordered_map<std::string, Directory*> subdirectories; 

public:
    Directory(const std::string& name, Directory* parent = nullptr);
    ~Directory();

    const std::string& getName() const;
    Directory* getParent() const;

    void createFile(const std::string& name, const std::string& readPassword = "", const std::string& writePassword = "");
    Directory* createSubdirectory(const std::string& name);

    Inode* findFile(const std::string& name) const;
    Directory* findSubdirectory(const std::string& name) const;

    void deleteFile(const std::string& name);
    void deleteSubdirectory(const std::string& name);

    std::vector<std::string> listContents() const;
};