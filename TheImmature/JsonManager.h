#pragma once
#include "Systems.h"
#include "JsonValidator.h"

// src/game/TextManager.h

class JsonManager {
private:

    ManifestData j_Data; // шаблон для хранения вводных данных manifest.json
    bool j_ManifestLoaded = false; // флаг загрузки, меняется если все данные успешно загружены, инача загрузка по умолчанию 

    std::vector <NPC> j_NPCs;
    bool j_NPCsLoaded = false;



    

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
    bool LoadAllNPCs();
    NPC LoadNPCFromFile(const std::string& filepath);

    EmotionData ParseEmotion(const json& emotionJson);
    LocationData ParseLocation(const json& locationJson);
    bool ParseManifest(const nlohmann::json& jsonData);
    bool LoadCheckManifest();

    DialogAnswer ParseAnswer(const json& answerJson);
    DialogText ParseDialogText(const json& textJson);

    void ClearManifestData();

    const EmotionData* GetEmotionByID(const std::wstring& id) const;
    const EmotionData* GetEmotionByIndex(size_t index) const;
    std::wstring GetEmotionDisplayName(Emotion_ emotion) const;

    bool AreNPCsLoaded() const { return j_NPCsLoaded; }
    const std::vector<NPC>& GetNPCs() const { return j_NPCs; }
    const ManifestData& GetData() const { return j_Data; }

    // Быстрые методы для поиска NPC
    const NPC* GetNPCByID(const std::wstring& id) const;
    std::vector<const NPC*> GetNPCsInWorld(Emotion_ world) const;
    size_t GetNPCCount() const { return j_NPCs.size(); }

    // Общие методы
    /*void LoadAllNPCs();
    vector<NPC*> GetNPCsInWorld(Emotion_ world);
    NPC* GetNPCByID(const string& npcID);
    bool HasNPCInWorld(Emotion_ world);*/

};