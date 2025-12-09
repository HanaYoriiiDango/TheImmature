#pragma once
#include "WinManager.h"    
#include "ResManager.h"
#include "ManifestManager.h"

// src/core/InitSystem.h

class InitSystem {
private:
    WindowManager& winManager;
    ResourceManager& resManager;
    ManifestManager& i_Manifest;


public:

    InitSystem(WindowManager& wm, ResourceManager& rm, ManifestManager& mm) 
        : winManager(wm), resManager(rm), i_Manifest(mm) {}

    //void Info();
    //void CreateWorlds();
    //void CreatePortals(Emotion_ WorldEmotion);
    bool WindowInitialize(HINSTANCE hInstance);
    bool BMPInitialize();
    bool ManifestInitialize();

};