#include <windows.h>
#include "JsonValidator.h"
#include "StringUtils.h"
#include <sstream>
#include <set>

// Логирование
void JsonValidator::LogError(const std::string& context, const std::string& details) { // вывод информации об ошибке 
    std::wstring message = StringUtils::UTF8ToWide("[" + context + "] " + details);
    OutputDebugStringW(message.c_str());
}

void JsonValidator::LogInfo(const std::string& context, const std::string& details) { // вывод информации о результате
    std::wstring message = StringUtils::UTF8ToWide("[INFO][" + context + "] " + details);
    OutputDebugStringW(message.c_str());

}

// Поиск файлов
std::wstring JsonValidator::FindDirectory(const std::wstring& directory) { // проверяет существование папки 

    if (!fs::exists(directory)) { // Если ее нет, то создает пустую
        fs::create_directories(directory);
        return directory;
    }

    return directory; // если она существует просто возвращаем ее же 
}

std::wstring JsonValidator::FindJson(const std::wstring& JsonPath) { // проверяет существует ли по указанному пути json

    if (fs::exists(JsonPath) && fs::is_regular_file(JsonPath)) { // если такой обычный файл существует, то возвращает его 
        return JsonPath;
    }

    return L""; // если нет то пустая строка 
}

std::wstring JsonValidator::FindFileInFolder(const std::wstring& folder, const std::wstring& filepath) { // Ищем файл в конкретной папке

    std::wstring basePath = FindDirectory(folder); // проверяем существование базовой папки, если что создаем
    std::wstring manifestPath = basePath + filepath; // ищем файл по базовому пути 
    if (FindJson(manifestPath) == L"") return L"";

    return manifestPath; // если все хорошо, то возвращаем путь до файла 
}

// Проверка ключей в файле

bool JsonValidator::IsStringKey(const json& j, const std::string& Key) { // проверяет существование ключа типа string

    if (!j.contains(Key) || !j[Key].is_string()) {
        LogError("IsStringKey", "Key" + Key + "not found");
        return false;
    }
    return true;
}

bool JsonValidator::IsArrayKey(const json& j, const std::string& Key) { // проверяет существование ключа-массива

    if (!j.contains(Key) || !j[Key].is_array()) {
        LogError("IsArrayKey", "Key" + Key + "not found");
        return false;
    }
    return true;
}

bool JsonValidator::IsIntegerKey(const json& j, const std::string& Key) { // проверяет существование ключа типа int 

    if (!j.contains(Key) || !j[Key].is_number_integer()) {
        LogError("IsIntegerKey", "Key" + Key + "not found");
        return false;
    }
    return true;
}

bool JsonValidator::IsBooleanKey(const json& j, const std::string& Key) { // проверяет существования ключа типа bool
    if (!j.contains(Key) || !j[Key].is_boolean()) {
        LogError("IsBooleanKey", "Key" + Key + "not found");
        return false;
    }
    return true;
}

bool JsonValidator::IsValueInRange(const json& j, const std::string& Key, int min, int max) { // проверяет диапозон значений для ключа

    if (!IsIntegerKey(j, Key)) {
        return false;  // IsIntegerKey залогирует ошибку
    }

    int value = j[Key].get<int>(); // если все хорошо проверяем значение 
    if (value < min || value > max) {
        std::string msg = "Value " + std::to_string(value) +
            " is outside range [" + std::to_string(min) +
            ", " + std::to_string(max) + "] for key '" + Key + "'";
        LogError("IsValueInRange", msg);
        return false;
    }
    return true;
}


// Проверки соответствия внутренним спискам 

bool JsonValidator::IsValidEmotionID(const std::wstring& id) {
    return StringToEmotion(id) < COUNT_Emotions;
    // Если конвертация вернула COUNT_Emotions - эмоция невалидна
}

// Конвертация 
Emotion_ JsonValidator::StringToEmotion(const std::wstring& id) { // просто возвращает значение из строки 
    // Простое преобразование
    if (id == L"JOY") return JOY;
    if (id == L"SADNESS") return SADNESS;
    if (id == L"POWER") return POWER;
    if (id == L"FEAR") return FEAR;
    if (id == L"CALM") return CALM;
    if (id == L"ANGER") return ANGER;
    return COUNT_Emotions; // По умолчанию - для обработки ошибок 
}


// Комплексные проверки 

bool JsonValidator::ValidateRequiredFields(const json& ArrayKey, const std::vector<std::string>& requiredKeys) {

    if (requiredKeys.empty()) { // проверяем массив ключей 
        LogError("ValidateRequiredFields", "Required keys vector is empty");
        return false; 
    } 

    for (const auto& field : requiredKeys) { // проверяем по указанным ключам
        if (!ArrayKey.contains(field)) {
            LogError("ValidateRequiredFields",
                "Missing required field: " + field);
            return false;
        }
    }
    return true;
}

bool JsonValidator::ValidateEmotion(const json& emotionJson) {
    // Проверяем обязательные поля
    if (!ValidateRequiredFields(emotionJson, EMOTION_FIELDS)) {
        return false;
    }

    //  Проверяем типы
    if (!IsStringKey(emotionJson, "id") ||
        !IsStringKey(emotionJson, "display_name") ||
        !IsIntegerKey(emotionJson, "default_value")) {
        return false;
    }

    //  Проверяем значение
    if (!IsValueInRange(emotionJson, "default_value", 0, 100)) {
        return false;
    }

    // Проверяем, что ID эмоции валиден
    std::wstring id = StringUtils::UTF8ToWide(emotionJson["id"].get<std::string>());
    if (!IsValidEmotionID(id)) {
        LogError("ValidateEmotion", "Invalid emotion ID");
        return false;
    }

    // Проверяем, что строки не пустые
    std::string emotionIdStr = emotionJson["id"].get<std::string>();
    std::wstring emotionIdWstr = StringUtils::UTF8ToWide(emotionIdStr);

    if (emotionIdStr.empty()) {
        LogError("ValidateEmotion", "Emotion ID cannot be empty");
        return false;
    }

    if (!IsValidEmotionID(emotionIdWstr)) {
        LogError("ValidateEmotion", "Invalid emotion ID: " + emotionIdStr);
        return false;
    }

    return true;
}

bool JsonValidator::ValidateWorld(const json& worldJson) {
    // 1. Обязательные поля
    if (!ValidateRequiredFields(worldJson, WORLD_FIELDS)) {
        return false;
    }

    // 2. Проверяем типы
    if (!IsStringKey(worldJson, "name") ||
        !IsStringKey(worldJson, "linked_emotion") ||
        !IsBooleanKey(worldJson, "locked_by_default")) {
        return false;
    }

    // 3. Проверяем linked_emotion
    std::wstring emotionID = StringUtils::UTF8ToWide(
        worldJson["linked_emotion"].get<std::string>());

    if (!IsValidEmotionID(emotionID)) {
        LogError("ValidateWorld", "Invalid linked_emotion ID");
        return false;
    }

    // 4. Проверяем, что имя мира не пустое
    std::string name = worldJson["name"].get<std::string>();
    if (name.empty()) {
        LogError("ValidateWorld", "World name cannot be empty");
        return false;
    }

    return true;
}

bool JsonValidator::ValidateManifest(const json& manifestJson) {
    // 1. Проверяем корневые поля
    if (!IsStringKey(manifestJson, "version")) { // Ключ с версией содержит значение в виде строки
        LogError("ValidateManifest:", "not found version");
        return false;
    }

    if (!IsArrayKey(manifestJson, "emotions")) { // проверяет есть ли массив эмоций
        LogError("ValidateManifest:", "not found emotions");
        return false;
    }

    if (!IsArrayKey(manifestJson, "worlds")) { // проверяем есть ли массив миров
        LogError("ValidateManifest:", "not found worlds");
        return false;
    }

    // 2. Проверяем все эмоции на соответсвтие внутреннего списка
    for (const auto& emotion : manifestJson["emotions"]) {
        if (!ValidateEmotion(emotion)) {
            LogError("ValidateManifest: ", "Emotion validation failed");
            return false;
        }
    }

    // 3. Проверяем все миры на соответствие внутренного списка
    for (const auto& world : manifestJson["worlds"]) {
        if (!ValidateWorld(world)) {
            LogError("ValidateManifest: ", "World validation failed");
            return false;
        }
    }

    // 4. Проверяем уникальность ID эмоций
    std::set<std::string> emotionIds;
    for (const auto& emotion : manifestJson["emotions"]) {
        std::string id = emotion["id"].get<std::string>();
        if (emotionIds.count(id) > 0) {
            LogError("ValidateManifest: ", "Duplicate emotion ID: " + id);
            return false;
        }
        emotionIds.insert(id);
    }

    LogInfo("ValidateManifest",
        "Manifest validated: " +
        std::to_string(manifestJson["emotions"].size()) + " emotions, " +
        std::to_string(manifestJson["worlds"].size()) + " worlds");

    return true;
}