// src/core/GameCore.h
#pragma once
#include "Systems.h"
#include "FileSystem.h"
#include "ResManager.h"
#include "JsonManager.h"
#include "JsonValidator.h"
#include "Logic.h"
#include "Render.h"
#include "Dialog.h"
#include "WinManager.h"
#include "Init.h"

class GameCore { // игровое ядро, все системы разделены по модулям 
private:
    // Игровое ядро должно именно ВЛАДЕТЬ своими модулями - композиция 
    //StatisticsCollector Collector;
    FileSystem FileSystem;
    ResourceManager ResManager;
    JsonManager JsonManager;
    RenderSystem Render;
    WindowManager WinManager;
    InitSystem Init;
    JsonValidator Validator;
    GameLogicSystem Logic;
    DialogSystem Dialog;

public:

    GameCore();

    bool InitGame(HINSTANCE hInstance);
    void Run();

   /* void StartGame();
    void EndGame();
    void Help();
    void ProcessCommand();*/

};
