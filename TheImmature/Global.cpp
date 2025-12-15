#include "Global.h" 

std::vector<Emotion_> Emotion{ JOY, SADNESS, POWER, FEAR, CALM, ANGER };
std::wstring Emotion_Names[COUNT_Emotions] = { L"Радость", L"Грусть", L"Сила", L"Страх", L"Спокойствие", L"Гнев" };
std::wstring Worlds_Names[COUNT_Emotions] = { L"Мир Радости", L"Мир Грусти", L"Мир Силы", L"Мир Страха", L"Мир Спокойствия", L"Мир Гнева" };
Player Hero;
Location Worlds[COUNT_Emotions];
std::vector<NPC> Characters;
GameSession game;

Window_ window;

NPC* currentDialogNPC = nullptr;
DialogText* currentDialogText = nullptr;
int selectedAnswerIndex = 0;
bool inDialog = false;
int currentDialogTextID = 0;

bool g_NeedAutoStartDialog; // флаг для автозапуска 