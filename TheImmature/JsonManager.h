#pragma once
#include "Systems.h"
#include "JsonValidator.h"
#include "ResManager.h"

// src/game/TextManager.h

class JsonManager {
private:
    bool j_NPCsLoaded = false;

    ResourceManager& j_ResManager;

    // Внутренние методы
    /*vector<string> FindWorldFolders();
    vector<string> FindJSONFiles(const string& folderPath);
    NPC LoadNPCFromFile(const string& filepath);
    DialogText ParseDialogText(const json& textJson);
    DialogAnswer ParseAnswer(const json& answerJson);
    Emotion_ StringToEmotion(const string& emotionStr);*/

public:

    JsonManager(ResourceManager& rm) : j_ResManager(rm) {}

    bool LoadAllNPCs();
    NPC LoadNPCFromFile(const std::string& filepath);
    DialogAnswer ParseAnswer(const json& answerJson);
    DialogText ParseDialogText(const json& textJson);
    std::vector<NPC*> GetNPCsInWorld(Emotion_ world);
    NPC* GetNPCByID(const std::wstring& npcID);
    bool HasNPCInWorld(Emotion_ world);

};