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
private:
    // === Битмапы ===
    HBITMAP hBack = nullptr;          // Фоновый битмап
    HBITMAP backScales = nullptr;     // Битмап для шкал
    HBITMAP backReplace = nullptr;    // Битмап для замен
    HBITMAP backHero = nullptr;       // Битмап для героя
    HBITMAP backCharacter = nullptr;  // Битмап для персонажа
    HBITMAP backMainText = nullptr;   // Битмап для основного текста

public: 
    ResourceManager() = default;
    ~ResourceManager() {
        Cleanup();  // Автоматическая очистка при уничтожении объекта
    }

    bool FindFiles(const wchar_t* filename);
    HBITMAP LoadBMP(const wchar_t* name);
    bool LoadAllBMP();
    void Cleanup();

    // Getters

    HBITMAP GethBack();
    HBITMAP GetBackScales();
    HBITMAP GetBackReplace();
    HBITMAP GetBackHero();
    HBITMAP GetBackCharacter();
    HBITMAP GetbackMainText();

};

class RenderSystem {
private: 
    ResourceManager& resManager;

    //Вспомогательные методы 
    wstring IntToWString(int value);
    int GetScaledX(int x, float scaleX);
    int GetScaledY(int y, float scaleY);
    int GetScaledSize(int size, float uiScale);

public:
    RenderSystem(ResourceManager& rm) : resManager(rm) {}

    void ShowText(
        const HDC& hdc, const wstring& text, 
        int base_x, float windowScaleX, 
        int base_y, float windowScaleY, 
        int base_font_size, float windowUiscale
    );

    void ShowBMP(
        const HDC& hdc,
        int base_x, float windowScaleX,
        int base_y, float windowScaleY,
        float windowUiscale , int base_w, int base_h,
        HBITMAP hBitmap, bool transparent = false
    );

    void ShowObject(const HDC& hdc, float windowScaleX, float windowScaleY, float windowScaleUI);

};

class WindowManager {
private:

    struct {

        // === Основные поля окна ===
        HWND hwnd = nullptr;
        HINSTANCE hInstance = nullptr;
        const wchar_t* className = L"Main";

        // === Размеры и масштабирование ===
        int width = 0;
        int height = 0;
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        float uiScale = 1.0f;

        // === Контексты устройств ===
        HDC hdc = nullptr, memDC = nullptr;   // Основной HDC и буфер 

        

    }window;

    RenderSystem& render;
    ResourceManager& resManager;
    
    // === Обработка сообщений ===
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    

public: 

    WindowManager(RenderSystem& rs, ResourceManager& rm) : render(rs), resManager(rm) {}
    
    // Настройка и создание окна
    bool WindowCreate();
    bool InitWindow();
    bool RegistrClass(HINSTANCE hInstance);
    void WinUpdate();
    void WhileMessage();

    // Остальное
    void Render();

};

class InitSystem { // инициализация 
private: 
    WindowManager& winManager;
    ResourceManager& resManager;

public:
    InitSystem(WindowManager& wm, ResourceManager& rm) : winManager(wm), resManager(rm) {}

    void Info();
    void CreateWorlds();
    void CreatePortals(Emotion_ WorldEmotion);
    bool WindowInitialize(HINSTANCE hInstance);
    bool BMPInitialize();

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
    //StatisticsCollector Collector;
    ResourceManager ResManager;
    RenderSystem Render;
    WindowManager WinManager;
    InitSystem Init;
    //TextManager Manager;
    //GameLogicSystem Logic;
    //DialogSystem Dialog;

    string temp;

public:

    GameCore()
        : Render(ResManager),  // ResourceManager уже создан
          WinManager(Render, ResManager),  // Оба созданы
          Init(WinManager, ResManager)    // Все созданы
    {}

    bool InitGame(HINSTANCE hInstance);
    void Update();
    void Run();
    void StartGame();
    void EndGame();
    void Help();
    void ProcessCommand();

};


