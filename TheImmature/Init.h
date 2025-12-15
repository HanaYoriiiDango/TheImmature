#pragma once
#include "WinManager.h"    
#include "ResManager.h"
#include "JsonManager.h"

// src/core/InitSystem.h

class InitSystem {
private:
    WindowManager& winManager;
    ResourceManager& resManager;
    JsonManager& i_jsonManager;


public:

    InitSystem(WindowManager& wm, ResourceManager& rm, JsonManager& jm)
        : winManager(wm), resManager(rm), i_jsonManager(jm){}

    //void Info();
    void CreateWorlds();
    void CreatePortals(Emotion_ WorldEmotion);
    bool WindowInitialize(HINSTANCE hInstance);
    bool BMPInitialize();
    //bool ManifestInitialize();
    bool DialogInitialize();

};