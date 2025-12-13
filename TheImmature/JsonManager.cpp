#include "JsonManager.h" 
#include "StringUtils.h" 

// Проверки на загрузки файлов:

void JsonManager::ProcessLoaded() { // можно дать альетрнативное название loadedJson?
    // конечная функция в которой будет описываться вся загрузка

    if (LoadManifest()) j_ManifestLoaded = true;

    // здесь оптимально будет проверят флаг и если он все еще false - загружать значения по умолчанию 

}

bool JsonManager::LoadManifest() {

    // сначало проверяем существование базовой папки + сам файл
    std::wstring fullpath = JsonValidator::FindFileInFolder(L"data/", L"manifest.json");

    if (fullpath != L"") { // если manifest.json нет - вернет  L""
        JsonValidator::LogError("LoadManifest: ", "manifest.json not found");
        return false;
    };

    // Нашли файл, проверяем ключи файла - должны строго соответствовать системным
    if (!JsonValidator::ValidateManifest(fullpath)) return false;

    // Если валидация прошла успешно, то парсим файл:
    if (!ParseManifest(fullpath)) {
        JsonValidator::LogError("LoadManifest: ", "the parsing process failed");
        return false;
    };

    //Если парсинг прошел успешно, то проверяем загрузку: 
    if (j_Data.Emotion.empty()) {
        JsonValidator::LogError("LoadManifest: ", "Emotion empty!");
        return false;
    }
    if (j_Data.Worlds.empty()) {
        JsonValidator::LogError("LoadManifest: ", "Worlds empty!");
        return false;
    }

    if (j_Data.Current_Ver != NULL) {
        JsonValidator::LogError("LoadManifest: ", "Current_Ver is NULL!");
        return false;
    }

    return true; // если все проверки прошли успешно

}


bool JsonManager::ParseManifest(const std::wstring& filepath) {
    // Конвертируем путь
    std::string narrowPath = StringUtils::WideToUTF8(filepath);
    std::ifstream file(narrowPath);

    if (!file.is_open()) { // проверяем получилось ли открыть файл
        JsonValidator::LogError("Couldn't open the file:", narrowPath);
        return false;
    }

    try {
        json jsonData;
        file >> jsonData;

        // Очищаем старые данные:
        j_Data.Emotion.clear();
        j_Data.Worlds.clear();

        // Валидация версии:
        if (!JsonValidator::IsStringKey(jsonData, "version")) {
            JsonValidator::LogError("ParseManifest:", "not found version key");
            return false;
        };

        // если ключ есть и он нужного значения, то парсим его 
        j_Data.Current_Ver = std::stod(jsonData["version"].get<std::string>());

        // Дополнительная валидация массивов:



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

EmotionData JsonManager::ParseEmotion(const json& emotionJson) {
    EmotionData emotion;
    emotion.ID = StringUtils::UTF8ToWide(emotionJson["id"].get<std::string>());
    emotion.Display_Name = StringUtils::UTF8ToWide(emotionJson["display_name"].get<std::string>());
    emotion.DefaultValue = emotionJson["default_value"].get<int>();
    return emotion;
}

LocationData JsonManager::ParseLocation(const json& locationJson) {
    LocationData location;
    location.name = StringUtils::UTF8ToWide(locationJson["name"].get<std::string>());
    location.link = StringUtils::UTF8ToWide(locationJson["linked_emotion"].get<std::string>());
    location.is_locked = locationJson["locked_by_default"].get<bool>();
    return location;
}