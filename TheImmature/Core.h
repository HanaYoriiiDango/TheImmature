// src/core/GameCore.h
#pragma once
#include "Systems.h"
#include "ResManager.h"
#include "JsonManager.h"
#include "JsonValidator.h"
#include "Logic.h"
#include "Render.h"
#include "WinManager.h"
#include "Init.h"

class GameCore { // игровое ядро, все системы разделены по модулям 
private:
    // Игровое ядро должно именно ВЛАДЕТЬ своими модулями - композиция 
    //StatisticsCollector Collector;
    ResourceManager c_ResManager;
    JsonManager c_JsonManager;
    RenderSystem c_Render;
    WindowManager c_WinManager;
    InitSystem c_Init;
    JsonValidator c_Validator;
    GameLogicSystem c_Logic;
    //DialogSystem Dialog;

public:

    GameCore();

    bool InitGame(HINSTANCE hInstance);
    void Run();

   /* void StartGame();
    void EndGame();
    void Help();
    void ProcessCommand();*/

};
