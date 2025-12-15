// Dialog.h
#pragma once
#include "Global.h"
#include "JsonManager.h"
#include "Logic.h"
#include "Render.h"

class DialogSystem {
private:
    JsonManager& d_JsonManager;
    GameLogicSystem& d_Logic;
    RenderSystem& d_Render;

    // Состояние диалога
    NPC* currentNPC;
    DialogText* currentText;
    int selectedAnswer;
    bool isActive;
    int currentTextID;
    bool isChoosingNPC; // Флаг выбора NPC
    std::vector<NPC*> availableNPCs; // Доступные NPC

public:
    DialogSystem(JsonManager& jsonManager, GameLogicSystem& logic, RenderSystem& render);

    // Основные методы
    void StartDialogInWorld(); // Начать диалог в текущем мире
    void ProcessInput(int keyCode); // Обработка клавиш
    void Render(HDC hdc); // Рендеринг

    bool IsActive() const { return isActive; }
    void Reset() { isActive = false; currentNPC = nullptr; }
};