#include "JsonManager.h" 
#include "StringUtils.h" 
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


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

DialogAnswer JsonManager::ParseAnswer(const json& answerJson) {
    DialogAnswer answer;
    answer.text = StringUtils::UTF8ToWide(answerJson["text"].get<std::string>());
    answer.emotion = JsonValidator::StringToEmotion(
        StringUtils::UTF8ToWide(answerJson["emotion"].get<std::string>()));
    answer.sign = answerJson["sign"].get<bool>();
    answer.next_text_id = answerJson["next_text_id"].get<int>();
    return answer;
}

DialogText JsonManager::ParseDialogText(const json& textJson) {
    DialogText text;
    text.id = textJson["id"].get<int>();
    text.text = StringUtils::UTF8ToWide(textJson["text"].get<std::string>());
    text.emotion = JsonValidator::StringToEmotion(
        StringUtils::UTF8ToWide(textJson["emotion"].get<std::string>()));
    text.sign = textJson["sign"].get<bool>();

    for (const auto& answerJson : textJson["answers"]) {
        text.answers.push_back(ParseAnswer(answerJson));
    }

    return text;
}

NPC JsonManager::LoadNPCFromFile(const std::string& filepath) {
    NPC npc;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        JsonValidator::LogError("JsonManager", "Cannot open NPC file: " + filepath);
        return npc;
    }

    try {
        json jsonData;
        file >> jsonData;

        npc.id = StringUtils::UTF8ToWide(jsonData["id"].get<std::string>());
        npc.name = StringUtils::UTF8ToWide(jsonData["name"].get<std::string>());

        std::wstring worldLink = StringUtils::UTF8ToWide(
            jsonData["world_link"].get<std::string>());
        npc.world_link = JsonValidator::StringToEmotion(worldLink);

        for (const auto& textJson : jsonData["texts"]) {
            npc.texts.push_back(ParseDialogText(textJson));
        }
    }
    catch (const std::exception& e) {
        JsonValidator::LogError("JsonManager::LoadNPCFromFile",
            "Exception: " + std::string(e.what()));
        return npc;
    }

    return npc;
}


bool JsonManager::LoadAllNPCs() {
    std::string basePath = "data/npcs/";
    if (!fs::exists(basePath)) {
        JsonValidator::LogInfo("JsonManager", "NPC folder not found: " + basePath);
        return false;
    }

    int loadedCount = 0;

    for (const auto& entry : fs::directory_iterator(basePath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            NPC npc = LoadNPCFromFile(entry.path().string());

            if (!npc.id.empty()) {
                j_NPCs.push_back(std::move(npc));
                loadedCount++;
                JsonValidator::LogInfo("JsonManager", "Loaded NPC: ");
            }
        }
    }

    j_NPCsLoaded = (loadedCount > 0);
    return j_NPCsLoaded;  // ← ДОБАВИТЬ!
}

// Быстрые методы для поиска NPC
const NPC* JsonManager::GetNPCByID(const std::wstring& id) const {
    for (const auto& npc : j_NPCs) {
        if (npc.id == id) {
            return &npc;
        }
    }
    return nullptr;
}

std::vector<const NPC*> JsonManager::GetNPCsInWorld(Emotion_ world) const {
    std::vector<const NPC*> result;
    for (const auto& npc : j_NPCs) {
        if (npc.world_link == world) {
            result.push_back(&npc);
        }
    }
    return result;
}
