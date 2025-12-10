//#include "ManifestManager.h"
//#include "StringUtils.h" // 

#include "ManifestManager.h"
#include "StringUtils.h"
#include <windows.h>

// Вспомогательные функции
std::vector<std::wstring> ManifestManager::FindManifestFiles() {
    std::vector<std::wstring> manifests;
    std::wstring basePath = L"Data/";

    if (!fs::exists(basePath)) {
        fs::create_directories(basePath);
        return manifests;
    }

    // Ищем конкретно manifest.json
    std::wstring manifestPath = basePath + L"manifest.json";
    if (fs::exists(manifestPath)) {
        manifests.push_back(manifestPath);
    }

    return manifests;
}

EmotionData ManifestManager::ParseEmotion(const json& emotionJson) {
    EmotionData emotion;
    emotion.ID = StringUtils::UTF8ToWide(emotionJson["id"].get<std::string>());
    emotion.Display_Name = StringUtils::UTF8ToWide(emotionJson["display_name"].get<std::string>());
    emotion.DefaultValue = emotionJson["default_value"].get<int>();
    return emotion;
}

LocationData ManifestManager::ParseLocation(const json& locationJson) {
    LocationData location;
    location.name = StringUtils::UTF8ToWide(locationJson["name"].get<std::string>());
    location.link = StringUtils::UTF8ToWide(locationJson["linked_emotion"].get<std::string>());
    location.is_locked = locationJson["locked_by_default"].get<bool>();
    return location;
}

bool ManifestManager::ParseManifest(const std::wstring& filepath) {
    // Конвертируем путь
    std::string narrowPath = StringUtils::WideToUTF8(filepath);
    std::ifstream file(narrowPath);

    if (!file.is_open()) {
        OutputDebugStringW((L"Не удалось открыть файл: " + filepath + L"\n").c_str());
        return false;
    }

    try {
        json jsonData;
        file >> jsonData;

        // Очищаем старые данные
        m_Data.Emotion.clear();
        m_Data.Worlds.clear();

        // Парсим версию
        m_Data.Current_Ver = std::stod(jsonData["version"].get<std::string>());

        // Парсим эмоции
        for (const auto& emotionJson : jsonData["emotions"]) {
            m_Data.Emotion.push_back(ParseEmotion(emotionJson));
        }

        // Парсим миры
        for (const auto& locationJson : jsonData["worlds"]) {
            m_Data.Worlds.push_back(ParseLocation(locationJson));
        }

        return true;

    }
    catch (const std::exception& e) {
        std::string errorMsg = "Ошибка парсинга JSON: ";
        errorMsg += e.what();
        MessageBoxA(NULL, errorMsg.c_str(), "ManifestManager Error", MB_ICONERROR);
        return false;
    }
}

bool ManifestManager::LoadManifest() {
    auto files = FindManifestFiles();

    if (files.empty()) {
        OutputDebugStringW(L"ManifestManager: В папке Data/ нет manifest.json\n");
        return false;
    }

    // Парсим файл (теперь данные сохраняются в m_Data)
    if (!ParseManifest(files[0])) {
        return false;
    }

    // Проверяем, что загрузилось
    if (m_Data.Emotion.empty()) {
        OutputDebugStringW(L"ManifestManager: Не удалось загрузить эмоции\n");
        return false;
    }

    m_Loaded = true;

    // Отладочный вывод
    std::wstring debugMsg = L"ManifestManager: Успешно загружено ";
    debugMsg += std::to_wstring(m_Data.Emotion.size());
    debugMsg += L" эмоций и ";
    debugMsg += std::to_wstring(m_Data.Worlds.size());
    debugMsg += L" миров\n";
    OutputDebugStringW(debugMsg.c_str());

    return true;
}

Emotion_ ManifestManager::StringToEmotion(const std::wstring& id) const {
    // Простое преобразование
    if (id == L"JOY") return JOY;
    if (id == L"SADNESS") return SADNESS;
    if (id == L"POWER") return POWER;
    if (id == L"FEAR") return FEAR;
    if (id == L"CALM") return CALM;
    if (id == L"ANGER") return ANGER;
    return JOY; // По умолчанию
}


































//EmotionData ManifestManager::ParseEmotion(const json& emotionjson) {
//    EmotionData emotions;
//
//    // JSON возвращает std::string (UTF-8), конвертируем в wstring (UTF-16)
//    emotions.ID = StringUtils::UTF8ToWide(emotionjson["id"].get<std::string>());
//    emotions.Display_Name = StringUtils::UTF8ToWide(emotionjson["display_name"].get<std::string>());
//    emotions.DefaultValue = emotionjson["default_value"].get<int>();
//
//    return emotions;
//}
//
//LocationData ManifestManager::ParseLocation(const json& locationjson) {
//    LocationData locations;
//
//    locations.name = StringUtils::UTF8ToWide(locationjson["name"].get<std::string>());
//    locations.link = StringUtils::UTF8ToWide(locationjson["linked_emotion"].get<std::string>());
//    locations.is_locked = locationjson["locked_by_default"].get<bool>();
//
//    return locations;
//}
//
//ManifestData ManifestManager::ParseManifest(const std::wstring& filepath) {
//    ManifestData data;
//
//    // Конвертируем wstring → string для ifstream
//    std::string narrowPath = StringUtils::WideToUTF8(filepath);
//    std::ifstream file(narrowPath);
//
//    if (!file.is_open()) {
//        // Можно добавить отладочный вывод
//        OutputDebugStringW(L"ManifestManager: Не удалось открыть файл ");
//        OutputDebugStringW(filepath.c_str());
//        OutputDebugStringW(L"\n");
//        return data;
//    }
//
//    try {
//        json jsonData;
//        file >> jsonData;
//
//        data.Current_Ver = jsonData["version"].get<double>();
//
//        for (const auto& emotionJson : jsonData["emotions"]) {
//            data.Emotion.push_back(ParseEmotion(emotionJson));
//        }
//
//        for (const auto& locationsJson : jsonData["worlds"]) {
//            data.Worlds.push_back(ParseLocation(locationsJson));
//        }
//
//    }
//    catch (const std::exception& e) {
//        std::string errorMsg = "Ошибка парсинга JSON: ";
//        errorMsg += e.what();
//        MessageBoxA(NULL, errorMsg.c_str(), "ManifestManager Error", MB_ICONERROR);
//    }
//
//    return data;
//}
//
//std::vector<std::wstring> ManifestManager::FindManifest() {
//    std::vector<std::wstring> manifests;
//    std::wstring basePath = L"Data/";
//
//    if (!fs::exists(basePath)) {
//        // Можно создать папку автоматически
//        fs::create_directories(basePath);
//        return manifests;
//    }
//
//    for (const auto& entry : fs::directory_iterator(basePath)) {
//        if (entry.is_regular_file() && entry.path().extension() == ".json") {
//            // Возвращаем ПОЛНЫЙ ПУТЬ, а не только имя файла!
//            manifests.push_back(entry.path().wstring());
//        }
//    }
//
//    return manifests;
//}
//
//bool ManifestManager::LoadManifest() {
//    auto files = FindManifest();
//    if (files.empty()) return false;
//
//    // Парсим во временную переменную
//    ManifestData newData = ParseManifest(files[0]);
//
//    // Проверяем результат
//    if (newData.Emotion.empty()) return false;
//
//    // Сохраняем в член класса
//    m_Data = std::move(newData); // или просто m_Data = newData;
//
//    // Проверяем соответствие enum
//    if (m_Data.Emotion.size() != COUNT_Emotions) {
//        OutputDebugStringW(L"Warning: Emotion count mismatch!\n");
//    }
//
//    return true;
//}
