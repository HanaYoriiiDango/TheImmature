#pragma once
#include "Global.h"
#include "JsonManager.h"

class RenderSystem;

class GameLogicSystem {
private:
    JsonManager* l_JsonManager = nullptr;
    RenderSystem* l_Render = nullptr; // Добавить ссылку на RenderSystem

    // Вспомогательные переменные
    Emotion_ ArrayNum;
    int dominationRate = 5;
    int passiveRate = 2;
    std::vector<Emotion_> Positive;
    std::vector<Emotion_> Negative;

    // Для выбора мира
    std::map<Emotion_, int> dialogsCompletedByWorld;
    bool isSelectingWorld = false;
    int selectedPortal = 0;

public:
    GameLogicSystem() : l_JsonManager(nullptr), l_Render(nullptr) {}

    void SetJsonManager(JsonManager* jm) { l_JsonManager = jm; }
    void SetRenderSystem(RenderSystem* render) { l_Render = render; } 

    Emotion_ GetOpposite(Emotion_ feels);
    bool LimitCheck(int value);
    bool HeroLocCheck();
    Emotion_ DetectedEmotion(int feels);
    void LockedValue(Emotion_ feels);
    void MovingPlayer();
    void ChangeGamerule();
    void Transfuse(Emotion_ feels);
    void Addition(Emotion_ feels, std::vector<Emotion_> Array);
    void Subtraction(Emotion_ feels, std::vector<Emotion_> Array);
    void ChangeEmotions(Emotion_ DominationEmotion, bool sign);

    // Новые методы для выбора мира
    int CountNPCsInWorld(Emotion_ world);
    void OnDialogCompleted(Emotion_ world);
    void OnDialogCompleted(); // Перегрузка

    void StartWorldSelection();
    void ProcessWorldSelection(int keyCode);
    void SelectWorld();
    void RenderWorldSelection(HDC hdc);

    void ResetDialogCounterForWorld(Emotion_ world);

    // Геттеры
    bool IsSelectingWorld() const { return isSelectingWorld; }

};