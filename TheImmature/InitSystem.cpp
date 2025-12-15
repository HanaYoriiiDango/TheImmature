// src/core/InitSystem.cpp
#include "Init.h"

// Реализации методов InitSystem 

bool InitSystem::WindowInitialize(HINSTANCE hInstance) {

    if (!winManager.RegistrClass(hInstance)) {
        MessageBox(NULL, L"Init Error", L"RegistrClass: hInstance | CLASS_NAME | RegisterClassEx - false", MB_ICONERROR);
        return false;

    }

    if (!winManager.WindowCreate()) {
        MessageBox(NULL, L"Init Error", L"WindowCreate: NULL HWND", MB_ICONERROR);
        return false;

    }

    if (!winManager.InitWindow()) {
        MessageBox(NULL, L"Init Error", L"InitWindow: NULL HWND", MB_ICONERROR);
        return false;
    }

    return true;
}

bool InitSystem::BMPInitialize() {

    if (!resManager.LoadAllBMP()) { 
        MessageBox(NULL, L"Init Error", L"Load BMP", MB_ICONERROR);
        return false;
    } 

    return true;

};

bool InitSystem::DialogInitialize() {




    return true;
}

void InitSystem::CreateWorlds() {

    for (int i = 0; i < Emotion.size(); i++) {

        Worlds[i].name = Worlds_Names[i];
        Worlds[i].linked_emotion = Emotion[i];

        Worlds[i].background = resManager.GetWorldBackground((Emotion_)i);

        // ⭐⭐⭐ Закрываем CALM и JOY для демо ⭐⭐⭐
        if (i == CALM || i == JOY) {
            Worlds[i].is_available = false;
            Worlds[i].is_locked = true;
        }
        else {
            Worlds[i].is_available = true;
            Worlds[i].is_locked = false;
        }

        CreatePortals(Worlds[i].linked_emotion);
        
    }
}

void InitSystem::CreatePortals(Emotion_ WorldEmotion) {

    for (int i = 0; i < Emotion.size(); i++) {

        if (WorldEmotion == i) {

            for (int j = 0; j < Emotion.size(); j++) {

                if (WorldEmotion != j) {

                    Worlds[WorldEmotion].portal.push_back({ Worlds_Names[j], Emotion[j] });

                }
            }
        }
    }

}