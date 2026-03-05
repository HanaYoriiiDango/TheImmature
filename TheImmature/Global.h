#pragma once
#include "Windows.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdint>

// Перечисления
enum Emotion_ { JOY, SADNESS, POWER, FEAR, CALM, ANGER, COUNT_Emotions }; // Список БАЗОВЫХ эмоций для удобства работы 

namespace Texture {

    enum class ID {

        // World background
        Background_JOY,
        Background_SAD, 
        Background_POWER,
        Background_FEAR,
        Background_CALM,
        Background_ANGER,

        // UI
        hBack,          // Фоновый битмап
        backScales,     // Битмап для шкал
        backReplace,    // Битмап для замен
        backHero,       // Битмап для героя
        backCharacter,  // Битмап для персонажа
        backMainText,   // Битмап для основного текста
        ButtonStart,    // кнопка старта 

        // Персонажи
        HeroGG,
        NpcLoran,
        NpcNimi,
        NpcElion,
        NpcDefault,

        AllDefault, // общая заглушка 

        Count  // размер массива
    };
}

enum GameState {
    MAIN_MENU,
    DIALOG,
    WORLD_SELECTION
};

// Структуры

struct Window_ {

    // Основные поля окна 
    HWND hwnd = nullptr;
    HINSTANCE hInstance = nullptr;
    const char* className = "Main";

    // Размеры и масштабирование 
    int width = 0;
    int height = 0;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float uiScale = 1.0f;

    // Контексты устройств
    HDC hdc = nullptr, memDC = nullptr;   // Основной HDC и буфер 



};

struct Portal_ {
    std::string name;
    Emotion_ target;
    bool open = true;
};

// Структура для ответов
struct DialogAnswer {
    std::string text;
    Emotion_ emotion;
    bool sign; // true = увеличить, false = уменьшить
    int next_text_id;
    bool show = false;
    HBITMAP back;
};

// Структура для текста NPC
struct DialogText {
    int id;
    std::string text;
    Emotion_ emotion;
    bool sign; // true = увеличить, false = уменьшить
    std::vector<DialogAnswer> answers;
    bool show = false;

};

// Структура для NPC
struct NPC {
    std::string id;
    std::string name;
    Emotion_ world_link; // Линк персонажа с миром 
    std::vector<DialogText> texts;
    HBITMAP icon = nullptr;
    bool show = false;

};

struct Player {
    int current_loc = ANGER;
    int emotions[COUNT_Emotions] = { 50, 50, 50, 50, 50, 50 };
    HBITMAP Icon = nullptr;
    bool icon_show = false;
    bool life = true;
};

struct Location {
    std::string name;
    Emotion_ linked_emotion;
    bool is_locked = false;
    std::vector<Portal_> portal;
    bool is_available = true; // Добавить
    HBITMAP background = nullptr; // Добавить

};


struct GameSession {
    GameState Current_State = MAIN_MENU;

    // Основная информация о сессии
    int ID; // номер сессии
    time_t startTime, endTime; // Начало сессии / конец сессии
    double TimeMin; // перевод в минутах

    // Основная статистика
    int worldVisitSad;        // Сколько раз посещал sadness
    int worldVisitJoy;        // Сколько раз посещал joy и т.д
    int worldVisitPower;
    int worldVisitFear;
    int worldVisitCalm;
    int worldVisitAnger;
    int AllVisitCount;        // Сколько раз перемещался в общем 

    // Статистика по диалогам
    int counterChoices;                  // Всего принятых диалоговых решений

};

extern Player Hero;
extern Location Worlds[COUNT_Emotions]; 
// Внешние объявления глобальных переменных
extern std::vector<Emotion_> Emotion;
extern std::string Emotion_Names[COUNT_Emotions];
extern std::string Worlds_Names[COUNT_Emotions];
extern std::vector<NPC> Characters; // Все NPC игры
extern GameSession game;
extern Window_ window;

extern NPC* currentDialogNPC;      // Текущий NPC в диалоге
extern DialogText* currentDialogText; // Текущая реплика в диалоге
extern int selectedAnswerIndex;    // Выбранный ответ (0-based)
extern bool inDialog;              // Флаг, что мы в диалоге
extern int currentDialogTextID;    // ID текущего текста диалога
extern bool g_NeedAutoStartDialog; // флаг для автозапуска 