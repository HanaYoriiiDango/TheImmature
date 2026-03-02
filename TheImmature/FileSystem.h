// FileSystem.h
#pragma once
#include "Global.h"
#include <filesystem>

namespace fs = std::filesystem;

class FileSystem {
public:

    // Поиск файлов
    static std::wstring FindDirectory(const std::wstring& data); // поиск папки 
    static std::wstring FindJson(const std::wstring& JsonPath); // поиск JSON 
    static std::wstring FindFileInFolder(const std::wstring& folder, const std::wstring& filepath); // поиск файла по конкретному пути 
    std::vector<std::string> FindAllFiles(const std::string& folderPath, const std::string& expansion);

};