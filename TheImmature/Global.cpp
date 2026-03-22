#include "Global.h" 

std::vector<Emotion_> Emotion{ JOY, SADNESS, POWER, FEAR, CALM, ANGER };
std::string Emotion_Names[COUNT_Emotions] = { "Радость", "Грусть", "Сила", "Страх", "Спокойствие", "Гнев" };
std::string Worlds_Names[COUNT_Emotions] = { "Мир Радости", "Мир Грусти", "Мир Силы", "Мир Страха", "Мир Спокойствия", "Мир Гнева" };
Player Hero;
Location Worlds[COUNT_Emotions];
UI Interface;

std::vector<NPC> Characters;
GameSession game;

Window_ window;

NPC* currentDialogNPC = nullptr;
DialogText* currentDialogText = nullptr;
int selectedAnswerIndex = 0;
bool inDialog = false;
int currentDialogTextID = 0;

bool g_NeedAutoStartDialog; // флаг для автозапуска 