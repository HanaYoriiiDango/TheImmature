#pragma once
#include "Global.h"
#include "JsonManager.h"

class GameLogicSystem {
private:
    Player l_Hero;
    JsonManager* l_JsonManager = nullptr;

    // вспом. переменные и массивы
    Emotion_ ArrayNum;
    int dominationRate = 5;
    int passiveRate = 2;
    vector<Emotion_> Positive;
    vector<Emotion_> Negative;

    vector<Emotion_> Emotion = { JOY, SADNESS, POWER, FEAR, CALM, ANGER };

public:
    GameLogicSystem() {
        // Инициализация стандартными значениями
        l_Hero = Player();
    }

    void SetJsonManager(JsonManager* jm) { l_JsonManager = jm; }

    Emotion_ GetOpposite(Emotion_ feels);
    bool LimitCheck(int value);
    bool HeroLocCheck();
    Emotion_ DetectedEmotion(int feels);
    void LockedWorlds();
    void UnlockedWorlds();
    void LockedValue(Emotion_ feels);
    void MovingPlayer();
    void ChangeGamerule();
    void Transfuse(Emotion_ feels);
    void Addition(Emotion_ feels, std::vector<Emotion_> Array);
    void Subtraction(Emotion_ feels, std::vector<Emotion_> Array);
    void ChangeEmotions(Emotion_ DominationEmotion, bool sign);
    void ProcessGo();
    void StatusInfo();


};