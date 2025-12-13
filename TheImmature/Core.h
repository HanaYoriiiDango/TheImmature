// src/core/GameCore.h
#pragma once
#include "Systems.h"
#include "ResManager.h"
#include "JsonValidator.h"
#include "Render.h"
#include "WinManager.h"
#include "JsonManager.h"
#include "Init.h"

class GameCore { // игровое ядро, все системы разделены по модулям 
private:
    // Игровое ядро должно именно ВЛАДЕТЬ своими модулями - композиция 
    //StatisticsCollector Collector;
    ResourceManager c_ResManager;
    JsonValidator c_Validator;
    RenderSystem c_Render;
    WindowManager c_WinManager;
    InitSystem c_Init;
    JsonManager c_Manager;
    //GameLogicSystem Logic;
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
