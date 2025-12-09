#pragma once
#include "Systems.h"
#include "Global.h"
#include <filesystem>
#include <fstream>
#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

// src/game/ManifestManager.h

class ManifestManager {

    enum Emotion_ { JOY, SADNESS, POWER, FEAR, CALM, ANGER, COUNT_Emotions }; // все базовые эмоции по умолчанию
    ManifestData m_Data;

public:

    ManifestManager() = default;

    EmotionData ParseEmotion(const json& emotionjson);
    LocationData ParseLocation(const json& locationjson);
    ManifestData ParseManifest(const wstring& filepath);
    vector <wstring> FindManifest();
    bool LoadManifest();

    // здесь пока нет методов, значит и в cpp нечего писать 

};