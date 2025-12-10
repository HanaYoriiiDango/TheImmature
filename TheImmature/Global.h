#pragma once
#include <vector>
#include <string>
#include <ctime> 

// Перечисления
enum Emotion_ { JOY, SADNESS, POWER, FEAR, CALM, ANGER, COUNT_Emotions }; // Список БАЗОВЫХ эмоций для удобства работы 

// Структуры

struct EmotionData {
    std::wstring ID;
    std::wstring Display_Name;
    int DefaultValue;
     
};

struct Portal_ {
    std::wstring name;
    int target;
    bool open = true;
};

struct LocationData {
    std::wstring name;
    std::wstring link; 
    bool is_locked;

};

struct ManifestData {

    double Current_Ver;
    std::vector<EmotionData> Emotion; // массив ВСЕХ эмоций что добавлены через json manifest
    std::vector<LocationData> Worlds; // массив всех миров описанных в json 

};


// Структура для ответов
struct DialogAnswer {
    std::wstring text;
    Emotion_ emotion;
    bool sign; // true = увеличить, false = уменьшить
    int next_text_id;
};

// Структура для текста NPC
struct DialogText {
    int id;
    std::wstring text;
    Emotion_ emotion;
    bool sign; // true = увеличить, false = уменьшить
    std::vector<DialogAnswer> answers;
};

// Структура для NPC
struct NPC {
    std::wstring id;
    std::wstring name;
    Emotion_ world_link; // Линк персонажа с миром 
    std::vector<DialogText> texts;
};

struct Player {
    int current_loc = SADNESS;
    int emotions[COUNT_Emotions] = { 50, 50, 50, 50, 50, 50 };
    bool life = true;
};

struct Location {
    std::wstring name;
    Emotion_ linked_emotion;
    bool is_locked = false;
    std::vector<Portal_> portal;

};


struct GameSession {
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
