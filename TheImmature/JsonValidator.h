
// JsonValidator.h
#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "Global.h"
#include "json.hpp"  // Просто подключаем здесь

namespace fs = std::filesystem;
using json = nlohmann::json;

class JsonValidator {
private:

    // Статические константы (одна копия на весь класс) - это список валидных ключей по умолчанию для контейнеров 
    static inline const std::vector<std::string> EMOTION_FIELDS = { // эмоции
        "id", "display_name", "default_value"
    };

    static inline const std::vector<std::string> WORLD_FIELDS = { // миры 
        "name", "linked_emotion", "locked_by_default"
    };

public: 

    // Поиск файлов
    static std::wstring FindDirectory(const std::wstring& data);
    static std::wstring FindJson(const std::wstring& JsonPath);
    static std::wstring FindFileInFolder(const std::wstring& folder, const std::wstring& filepath);

    // Проверки типов ключей 
    static bool IsStringKey(const json& j, const std::string& Key);
    static bool IsArrayKey(const json& j, const std::string& Key);
    static bool IsIntegerKey(const json& j, const std::string& Key);
    static bool IsBooleanKey(const json& j, const std::string& Key);

    // проверки значений 
    static bool IsValueInRange(const json& j, const std::string& Key, int min = 0, int max = 100);
    static bool IsValidEmotionID(const std::wstring& id);

    // Комплексные проверки
    static bool ValidateRequiredFields(const json& ArrayKey, const std::vector<std::string>& requiredKeys);
    static bool ValidateEmotion(const json& emotionJson);
    static bool ValidateWorld(const json& worldJson);
    static bool ValidateManifest(const json& manifestJson);

    // Концертация
    static Emotion_ StringToEmotion(const std::wstring& id);

    // Логирование 
    static void LogError(const std::string& context, const std::string& details = "");
    static void LogInfo(const std::string& context, const std::string& details = "");



};
