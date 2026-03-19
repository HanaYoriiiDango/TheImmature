#include "JsonManager.h" 
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


// Проверки на загрузки файлов:

DialogAnswer JsonManager::ParseAnswer(const json& answerJson) {
    DialogAnswer answer;
    answer.text = answerJson["text"].get<std::string>();
    answer.emotion = JsonValidator::StringToEmotion(answerJson["emotion"].get<std::string>());
    answer.sign = answerJson["sign"].get<bool>();
    answer.next_text_id = answerJson["next_text_id"].get<int>();
    return answer;
}

DialogText JsonManager::ParseDialogText(const json& textJson) {
    DialogText text;
    text.id = textJson["id"].get<int>();
    text.text = textJson["text"].get<std::string>();
    text.emotion = JsonValidator::StringToEmotion(textJson["emotion"].get<std::string>());
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

        npc.id = jsonData["id"].get<std::string>();
        npc.name = jsonData["name"].get<std::string>();
        npc.icon = j_ResManager.LoadBmpNpcs(npc.id);

        std::string worldLink = jsonData["world_link"].get<std::string>();
        npc.world_link = JsonValidator::StringToEmotion(worldLink);

        for (const auto& textJson : jsonData["texts"]) {
            npc.texts.push_back(ParseDialogText(textJson));
        }
    }
    catch (const std::exception& e) {
        std::string error = "[JSON] Ошибка парсинга " + filepath + ": " + e.what();
        OutputDebugStringA(error.c_str());  
        JsonValidator::LogError("JsonManager::LoadNPCFromFile", error);
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
                Characters.push_back(std::move(npc));
                loadedCount++;
                JsonValidator::LogInfo("JsonManager", "Loaded NPC: ");
            }
        }
    }

    j_NPCsLoaded = (loadedCount > 0);
    return j_NPCsLoaded; 
}

std::vector<NPC*> JsonManager::GetNPCsInWorld(Emotion_ world) {
    std::vector<NPC*> result;

    for (NPC& npc : Characters) {
        if (npc.world_link == world) {
            result.push_back(&npc);
        }
    }

    return result;
}

bool JsonManager::HasNPCInWorld(Emotion_ world) {
    return !GetNPCsInWorld(world).empty();
}

NPC* JsonManager::GetNPCByID(const std::string& npcID) {
    for (NPC& npc : Characters) {
        if (npc.id == npcID) {
            return &npc;
        }
    }
    return nullptr; // не найден
}