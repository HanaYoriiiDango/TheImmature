#pragma once
#include "Systems.h"
#include "JsonValidator.h"

// src/game/TextManager.h

class JsonManager {
private:

    ManifestData j_Data; // шаблон для хранения вводных данных manifest.json
    bool j_ManifestLoaded = false; // флаг загрузки, меняется если все данные успешно загружены, инача загрузка по умолчанию 
    

    // Внутренние методы
    /*vector<string> FindWorldFolders();
    vector<string> FindJSONFiles(const string& folderPath);
    NPC LoadNPCFromFile(const string& filepath);
    DialogText ParseDialogText(const json& textJson);
    DialogAnswer ParseAnswer(const json& answerJson);
    Emotion_ StringToEmotion(const string& emotionStr);*/

public:

    JsonManager() = default;

    bool LoadManifest(const std::wstring& customPath = L"data/manifest.json");
    EmotionData ParseEmotion(const json& emotionJson);
    LocationData ParseLocation(const json& locationJson);
    bool ParseManifest(const nlohmann::json& jsonData);
    bool LoadCheckManifest();

    void ClearManifestData();

    const EmotionData* GetEmotionByID(const std::wstring& id) const;
    const EmotionData* GetEmotionByIndex(size_t index) const;
    std::wstring GetEmotionDisplayName(Emotion_ emotion) const;

    // Общие методы
    /*void LoadAllNPCs();
    vector<NPC*> GetNPCsInWorld(Emotion_ world);
    NPC* GetNPCByID(const string& npcID);
    bool HasNPCInWorld(Emotion_ world);*/

};