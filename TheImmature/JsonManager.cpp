#include "JsonManager.h" 
#include "StringUtils.h" 

// Проверки на загрузки файлов:

bool JsonManager::LoadCheckManifest() {

    //Если парсинг прошел успешно, то проверяем загрузку: 
    if (j_Data.Emotion.empty()) {
        JsonValidator::LogError("JsonManager::LoadManifest", "No emotions loaded");
        return false;
    }
    if (j_Data.Worlds.empty()) {
        JsonValidator::LogError("JsonManager::LoadManifest", "No Worlds loaded");
        return false;
    }

    if (j_Data.Current_Ver == 0.0) {
        JsonValidator::LogError("JsonManager::LoadManifest", "Version not loaded");
        return false;
    }

    return true; // если все успешно загружено

}

bool JsonManager::LoadManifest(const std::wstring& customPath) {
    // Находим файл
    std::wstring fullpath;
    if (customPath.empty()) {
        fullpath = JsonValidator::FindFileInFolder(L"data", L"manifest.json");
    }
    else {
        fullpath = customPath; // если указал свой путь
    }

    // Проверяем, нашли ли файл
    if (fullpath.empty()) {
        JsonValidator::LogError("JsonManager::LoadManifest", "manifest.json not found in data/ folder");
        return false;
    }

    JsonValidator::LogInfo("JsonManager::LoadManifest", "Found manifest at: " + StringUtils::WideToUTF8(fullpath));

    // 2. Загружаем JSON
    std::string narrowPath = StringUtils::WideToUTF8(fullpath);
    std::ifstream file(narrowPath);

    if (!file.is_open()) {
        JsonValidator::LogError("JsonManager::LoadManifest", "Cannot open file: " + narrowPath);
        return false;
    }

    try {
        // 3. Читаем JSON
        nlohmann::json jsonData;
        file >> jsonData;
        file.close();

        // 4. ВАЛИДИРУЕМ весь манифест
        if (!JsonValidator::ValidateManifest(jsonData)) {
            JsonValidator::LogError("JsonManager::LoadManifest", "Manifest validation failed");
            return false;
        }

        JsonValidator::LogInfo("JsonManager::LoadManifest", "Manifest validation passed");

        // 5. ПАРСИМ данные (они гарантированно валидны)
        if (!ParseManifest(jsonData)) {
            JsonValidator::LogError("JsonManager::LoadManifest", "Failed to parse manifest data");
            return false;
        }

        // 6. Проверяем результат парсинга
        if (!LoadCheckManifest()) {
            JsonValidator::LogError("JsonManager::LoadCheckManifest", "manifest not loaded");
            return false;
        }

        // 7. Устанавливаем флаг
        j_ManifestLoaded = true;

        JsonValidator::LogInfo("JsonManager::LoadManifest",
            "Successfully loaded: " +
            std::to_string(j_Data.Emotion.size()) + " emotions, " +
            std::to_string(j_Data.Worlds.size()) + " worlds, " +
            "version " + std::to_string(j_Data.Current_Ver));

        return true;

    }
    catch (const nlohmann::json::parse_error& e) {
        // Специфичная обработка ошибок парсинга JSON
        std::string error = "JSON parse error at line " +
            std::to_string(e.byte) + ": " + e.what();
        JsonValidator::LogError("JsonManager::LoadManifest", error);
        return false;

    }
    catch (const std::exception& e) {
        // Общая обработка
        JsonValidator::LogError("JsonManager::LoadManifest",
            "Exception: " + std::string(e.what()));
        return false;
    }
}


bool JsonManager::ParseManifest(const nlohmann::json& jsonData) {
    try {
        // Очищаем старые данные
        ClearManifestData();

        // Парсим версию
        j_Data.Current_Ver = std::stod(jsonData["version"].get<std::string>());

        // Парсим эмоции
        for (const auto& emotionJson : jsonData["emotions"]) {
            j_Data.Emotion.push_back(ParseEmotion(emotionJson));
        }

        // Парсим миры
        for (const auto& locationJson : jsonData["worlds"]) {
            j_Data.Worlds.push_back(ParseLocation(locationJson));
        }

        return true;

    }
    catch (const std::exception& e) {
        JsonValidator::LogError("JsonManager::ParseManifest",
            "Exception: " + std::string(e.what()));
        return false;
    }
}

// так что в теории можно и не менять парсинг для эмоций и миров, так вся валидация пройдена

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

void JsonManager::ClearManifestData() {
    j_Data = ManifestData(); // Создаем новый пустой объект
    j_ManifestLoaded = false;
}

const EmotionData* JsonManager::GetEmotionByID(const std::wstring& id) const {
    for (const auto& emotion : j_Data.Emotion) {
        if (emotion.ID == id) {
            return &emotion;
        }
    }
    return nullptr;
}

const EmotionData* JsonManager::GetEmotionByIndex(size_t index) const {
    if (index < j_Data.Emotion.size()) {
        return &j_Data.Emotion[index];
    }
    return nullptr;
}

std::wstring JsonManager::GetEmotionDisplayName(Emotion_ emotion) const {
    // Преобразуем Emotion_ в строку ID
    std::wstring emotionID;
    switch (emotion) {
    case JOY: emotionID = L"JOY"; break;
    case SADNESS: emotionID = L"SADNESS"; break;
    case POWER: emotionID = L"POWER"; break;
    case FEAR: emotionID = L"FEAR"; break;
    case CALM: emotionID = L"CALM"; break;
    case ANGER: emotionID = L"ANGER"; break;
    default: emotionID = L"UNKNOWN";
    }

    // Ищем в загруженных данных
    const EmotionData* data = GetEmotionByID(emotionID);
    if (data) {
        return data->Display_Name;
    }

    return L"Unknown";
}
