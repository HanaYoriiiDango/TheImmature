// FileSystem.h
#pragma once
#include "Global.h"
#include <filesystem>

namespace fs = std::filesystem;

class FileSystem {
public:

    // Поиск файлов
    static std::string FindDirectory(const std::string& data); // поиск папки 
    static std::string FindJson(const std::string& JsonPath); // поиск JSON 
    static std::string FindFileInFolder(const std::string& folder, const std::string& filepath); // поиск файла по конкретному пути 
    std::vector<std::string> FindAllFiles(const std::string& folderPath, const std::string& expansion);

};