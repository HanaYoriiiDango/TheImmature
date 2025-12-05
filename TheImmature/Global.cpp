#include "Global.h"

vector<Emotion_> Emotion{ JOY, SADNESS, POWER, FEAR, CALM, ANGER };
wstring Emotion_Names[COUNT_Emotions] = { L"Радость", L"Грусть", L"Сила", L"Страх", L"Спокойствие", L"Гнев" };
wstring Worlds_Names[COUNT_Emotions] = { L"Мир Радости", L"Мир Грусти", L"Мир Силы", L"Мир Страха", L"Мир Спокойствия", L"Мир Гнева" };
Player Hero;
Window window;
Location Worlds[COUNT_Emotions];
vector<NPC> Characters;
