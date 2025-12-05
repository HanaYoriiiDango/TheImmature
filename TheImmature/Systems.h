#pragma once
#include "Global.h"
#include "json.hpp"
#include <fstream>

extern class GameCore* g_pGameCore;

using json = nlohmann::json;


class TextManager {
public:
    // Общие методы
    void LoadAllNPCs();
    vector<NPC*> GetNPCsInWorld(Emotion_ world);
    NPC* GetNPCByID(const string& npcID);
    bool HasNPCInWorld(Emotion_ world);

private:
    // Внутренние методы
    vector<string> FindWorldFolders();
    vector<string> FindJSONFiles(const string& folderPath);
    NPC LoadNPCFromFile(const string& filepath);
    DialogText ParseDialogText(const json& textJson);
    DialogAnswer ParseAnswer(const json& answerJson);
    Emotion_ StringToEmotion(const string& emotionStr);
};

class ResourceManager {
public: 
    







};

class RenderSystem {





};

class WindowManager {
public: 
    bool WindowCreate();
    bool InitWindow();
    bool RegistrClass(HINSTANCE hInstance);
    void WinUpdate();
    void WhileMessage();

};

class InitSystem { // инициализация 
private: 
    WindowManager& winManager;

public:
    InitSystem(WindowManager& wm) : winManager(wm) {}

    void Info();
    void CreateWorlds();
    void CreatePortals(Emotion_ WorldEmotion);
    bool WindowInitialize(HINSTANCE hInstance);

};

class StatisticsCollector {
public:
    GameSession session;
    ofstream SaveStatistics;
    void StartSession();
    void EndSession();
    void SaveData();
    void ClearStatistics();
    void RecordVisit();

};

class GameLogicSystem { // игровая логика
private:
    //зависимости
    StatisticsCollector& Collector;

    // вспом. переменные и массивы
    Emotion_ ArrayNum;
    int dominationRate = 5;
    int passiveRate = 2;
    vector<Emotion_> Positive;
    vector<Emotion_> Negative;

public:
    GameLogicSystem(StatisticsCollector& collector) : Collector(collector) {}

    Emotion_ GetOpposite(Emotion_ feels);
    bool LimitCheck(int value);
    bool HeroLocCheck();
    void LockedWorlds();
    void UnlockedWorlds();
    void LockedValue(Emotion_ feels);
    Emotion_ DetectedEmotion(int feels);
    void MovingPlayer();
    void ChangeGamerule();
    void Transfuse(Emotion_ feels);
    void Addition(Emotion_ feels, vector<Emotion_> Array);
    void Subtraction(Emotion_ feels, vector<Emotion_> Array);
    void ChangeEmotions(Emotion_ DominationEmotion, bool sign);
    void ProcessGo();
    void StatusInfo();

};

class DialogSystem {
private:
    // Зависимости:
    TextManager& textManager;
    GameLogicSystem& gameLogic;
    StatisticsCollector& statsCollector;

public:
    DialogSystem(TextManager& tm, GameLogicSystem& gl, StatisticsCollector& sc)
        :textManager(tm), gameLogic(gl), statsCollector(sc) {
    };

    void RunDialog(NPC* npc);
    void ProcessDialog();


};

class GameCore { // игровое ядро, все системы разделены по модулям 
private:
    // Игровое ядро должно именно ВЛАДЕТЬ своими модулями - композиция 
    StatisticsCollector Collector;
    InitSystem Init;
    TextManager Manager;
    GameLogicSystem Logic;
    DialogSystem Dialog;
    WindowManager WinManager;

    string temp;

public:

    GameCore()
        : Logic(Collector),
        Dialog(Manager, Logic, Collector),
        Init(WinManager)
    {
    }

    bool InitGame(HINSTANCE hInstance);
    void Update();
    void Run();
    void StartGame();
    void EndGame();
    void Help();
    void ProcessCommand();

};

