#pragma once
#include "WinManager.h"    
#include "ResManager.h"
#include "JsonValidator.h"

// src/core/InitSystem.h

class InitSystem {
private:
    WindowManager& winManager;
    ResourceManager& resManager;
    JsonValidator& i_Validator;


public:

    InitSystem(WindowManager& wm, ResourceManager& rm, JsonValidator& jv)
        : winManager(wm), resManager(rm), i_Validator(jv) {}

    //void Info();
    //void CreateWorlds();
    //void CreatePortals(Emotion_ WorldEmotion);
    bool WindowInitialize(HINSTANCE hInstance);
    bool BMPInitialize();
    bool ManifestInitialize();

};