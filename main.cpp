#include <iostream>
#include <string>
#include <sstream>
#include "fs.h"  


void printMenu() 
{
    std::cout << "File System Shell\n";
    std::cout << "Commands:\n";
    std::cout << "  create <filename>          - Создать файл\n";
    std::cout << "  delete <filename>          - Удалить файл\n";
    std::cout << "  open <filename>            - Открыть файл\n";
    std::cout << "  write <filename> <text>    - Записать в файл\n";
    std::cout << "  read <filename>            - Прочитать файл\n";
    std::cout << "  list                       - Показать все файлы\n";
    std::cout << "  exit                       - Выйти из оболочки\n";
}

int main() {
    PoolAllocator allocator; 
    FileSystem fs(&allocator); 

    std::string command;
    std::string filename;
    std::string data;

    printMenu();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "create") {
            iss >> filename;
            if (filename.empty()) 
            {
                std::cout << "Error: No filename specified.\n";
            } 
            else 
            {
                if (fs.createFile(filename)) 
                {
                    std::cout << "File " << filename << " created successfully.\n";
                } 
                else 
                {
                    std::cout << "Error: File already exists.\n";
                }
            }
        }
        else if (cmd == "delete") 
        {
            iss >> filename;
            if (filename.empty()) 
            {
                std::cout << "Error: No filename specified.\n";
            } 
            else 
            {
                if (fs.deleteFile(filename)) 
                {
                    std::cout << "File " << filename << " deleted successfully.\n";
                } 
                else 
                {
                    std::cout << "Error: File not found.\n";
                }
            }
        }
        else if (cmd == "open") 
        {
            iss >> filename;
            if (filename.empty()) 
            {
                std::cout << "Error: No filename specified.\n";
            } 
            else 
            {
                auto inode = fs.openFile(filename);
                if (inode) 
                {
                    std::cout << "File " << filename << " opened successfully.\n";
                } 
                else 
                {
                    std::cout << "Error: File not found.\n";
                }
            }
        }
        else if (cmd == "write") 
        {
            iss >> filename;
            std::getline(iss, data); 
            if (filename.empty() || data.empty()) 
            {
                std::cout << "Error: No filename or data specified.\n";
            } 
            else 
            {
                auto inode = fs.openFile(filename);
                if (inode) 
                {
                    inode->writeData(data.c_str(), data.size(), "");
                    std::cout << "Data written to " << filename << ".\n";
                } 
                else 
                {
                    std::cout << "Error: File not found.\n";
                }
            }
        }
        else if (cmd == "read") 
        {
            iss >> filename;
            if (filename.empty()) 
            {
                std::cout << "Error: No filename specified.\n";
            } else 
            {
                auto inode = fs.openFile(filename);
                if (inode) 
                {
                    char buffer[BLOCK_SIZE]; 
                    size_t bytesRead = inode->readData(buffer, sizeof(buffer), "");
                    std::cout << "Read " << bytesRead << " bytes: " << std::string(buffer, bytesRead) << "\n";
                } 
                else 
                {
                    std::cout << "Error: File not found.\n";
                }
            }
        }
        else if (cmd == "list") 
        {
            fs.listFiles();
        }
        else if (cmd == "exit") 
        {
            std::cout << "Exiting the shell.\n";
            break; 
        }
        else 
        {
            std::cout << "Error: Unknown command.\n";
        }
    }

    return 0;
}