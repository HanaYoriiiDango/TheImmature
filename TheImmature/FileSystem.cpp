#include "FileSystem.h"

// Поиск файлов
std::string FileSystem::FindDirectory(const std::string& directory) { // проверяет существование папки 

    if (!fs::exists(directory)) { // Если ее нет, то создает пустую
        fs::create_directories(directory);
        return directory;
    }

    return directory; // если она существует просто возвращаем ее же 
}

std::string FileSystem::FindJson(const std::string& JsonPath) { // проверяет существует ли по указанному пути json

    if (fs::exists(JsonPath) && fs::is_regular_file(JsonPath)) { // если такой обычный файл существует, то возвращает его 
        return JsonPath;
    }

    return ""; // если нет то пустая строка 
}

std::string FileSystem::FindFileInFolder(const std::string& folder, const std::string& filepath) { // Ищем файл в конкретной папке

    std::string basePath = FindDirectory(folder); // проверяем существование базовой папки, если что создаем
    std::string manifestPath = basePath + filepath; // ищем файл по базовому пути 
    if (FindJson(manifestPath) == "") return "";

    return manifestPath; // если все хорошо, то возвращаем путь до файла 

}

std::vector<std::string> FileSystem::FindAllFiles(const std::string& folderPath, const std::string& expansion) { // перебирает в папке все файлы с указанным расширеним 
    std::vector<std::string> files;

    if (!fs::exists(folderPath)) return files;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == expansion) {
            files.push_back(entry.path().filename().string());
        }
    }

    return files;
}
