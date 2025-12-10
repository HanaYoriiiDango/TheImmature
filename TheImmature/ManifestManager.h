
// ManifestManager.h
#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "Global.h"
#include "json.hpp"  // Просто подключаем здесь

namespace fs = std::filesystem;
using json = nlohmann::json;

class ManifestManager {
private:
    ManifestData m_Data;
    bool m_Loaded = false;

public:
    ManifestManager() = default;

    // Основной метод - возвращает true/false
    bool LoadManifest();

    // Проверка состояния
    bool IsLoaded() const { return m_Loaded; }

    // Получить данные
    const ManifestData& GetData() const { return m_Data; }

    // Вспомогательные методы
    Emotion_ StringToEmotion(const std::wstring& id) const;

private:
    // Внутренние методы
    std::vector<std::wstring> FindManifestFiles();
    bool ParseManifest(const std::wstring& filepath); // Теперь возвращает bool
    EmotionData ParseEmotion(const json& emotionJson);
    LocationData ParseLocation(const json& locationJson);
};














//#pragma once
//#include "Systems.h"
//#include "Global.h"
//#include <filesystem>
//#include <fstream>
//#include "json.hpp"
//
//namespace fs = std::filesystem;
//using json = nlohmann::json;
//
//// src/game/ManifestManager.h


//class ManifestManager {
//
//    ManifestData m_Data;
//    bool m_Loaded = false;
//    std::vector<std::wstring> FindManifestFiles();
//
//
//public:
//
//    ManifestManager() = default;
//
//    EmotionData ParseEmotion(const json& emotionjson);
//    LocationData ParseLocation(const json& locationjson);
//    ManifestData ParseManifest(const std::wstring& filepath);
//    std::vector <std::wstring> FindManifest();
//    bool LoadManifest();
//
//    bool IsLoaded() const { return m_Loaded; }
//
//    // здесь пока нет методов, значит и в cpp нечего писать 
//
//};