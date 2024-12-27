#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "directory.h"


class FileSystem 
{
private:
    Directory* root;      
    Directory* current;   
public:
    FileSystem();
    ~FileSystem();

    void createFile(const std::string& name, const std::string& readPassword = "", const std::string& writePassword = "");
    void createDirectory(const std::string& name);
    void changeDirectory(const std::string& path);
    void listDirectory() const;

    std::string getCurrentPath() const; 
};
