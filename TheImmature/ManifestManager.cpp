#include "ManifestManager.h"
#include "StringUtils.h" // 

EmotionData ManifestManager::ParseEmotion(const json& emotionjson) {
    EmotionData emotions;

    // JSON возвращает std::string (UTF-8), конвертируем в wstring (UTF-16)
    emotions.ID = StringUtils::UTF8ToWide(emotionjson["id"].get<std::string>());
    emotions.Display_Name = StringUtils::UTF8ToWide(emotionjson["display_name"].get<std::string>());
    emotions.DefaultValue = emotionjson["default_value"].get<int>();

    return emotions;
}

LocationData ManifestManager::ParseLocation(const json& locationjson) {
    LocationData locations;

    locations.name = StringUtils::UTF8ToWide(locationjson["name"].get<std::string>());
    locations.link = StringUtils::UTF8ToWide(locationjson["linked_emotion"].get<std::string>());
    locations.is_locked = locationjson["locked_by_default"].get<bool>();

    return locations;
}

ManifestData ManifestManager::ParseManifest(const std::wstring& filepath) {
    ManifestData data;

    // Конвертируем wstring → string для ifstream
    std::string narrowPath = StringUtils::WideToUTF8(filepath);
    std::ifstream file(narrowPath);

    if (!file.is_open()) {
        // Можно добавить отладочный вывод
        OutputDebugStringW(L"ManifestManager: Не удалось открыть файл ");
        OutputDebugStringW(filepath.c_str());
        OutputDebugStringW(L"\n");
        return data;
    }

    try {
        json jsonData;
        file >> jsonData;

        data.Current_Ver = jsonData["version"].get<double>();

        for (const auto& emotionJson : jsonData["emotions"]) {
            data.Emotion.push_back(ParseEmotion(emotionJson));
        }

        for (const auto& locationsJson : jsonData["worlds"]) {
            data.Worlds.push_back(ParseLocation(locationsJson));
        }

    }
    catch (const std::exception& e) {
        std::string errorMsg = "Ошибка парсинга JSON: ";
        errorMsg += e.what();
        MessageBoxA(NULL, errorMsg.c_str(), "ManifestManager Error", MB_ICONERROR);
    }

    return data;
}

std::vector<std::wstring> ManifestManager::FindManifest() {
    std::vector<std::wstring> manifests;
    std::wstring basePath = L"Data/";

    if (!fs::exists(basePath)) {
        // Можно создать папку автоматически
        fs::create_directories(basePath);
        return manifests;
    }

    for (const auto& entry : fs::directory_iterator(basePath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            // Возвращаем ПОЛНЫЙ ПУТЬ, а не только имя файла!
            manifests.push_back(entry.path().wstring());
        }
    }

    return manifests;
}

bool ManifestManager::LoadManifest() {
    auto files = FindManifest();

    if (files.empty()) {
        OutputDebugStringW(L"ManifestManager: В папке Data/ нет JSON файлов\n");
        return false;
    }

    // Берём первый найденный JSON файл
    m_Data = ParseManifest(files[0]);

    // Проверяем, что загрузилось
    if (m_Data.Emotion.empty()) {
        OutputDebugStringW(L"ManifestManager: Не удалось загрузить эмоции\n");
        return false;
    }

    OutputDebugStringW(L"ManifestManager: Успешно загружено ");
    OutputDebugStringW(std::to_wstring(m_Data.Emotion.size()).c_str());
    OutputDebugStringW(L" эмоций\n");

    return true;
}
