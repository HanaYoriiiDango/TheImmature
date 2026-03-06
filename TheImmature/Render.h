#pragma once
#include "Global.h"
#include "JsonValidator.h"
#include "ResManager.h"
#include <vector>
#include <string>
#include <algorithm>
#include <wingdi.h> 

// src/render/RenderSystem.h

class JsonManager;  
class DialogSystem; 

class RenderSystem {
private:
    ResourceManager& r_resManager;
    const JsonManager* r_jsonManager = nullptr; // Заменили ManifestManager
    DialogSystem* dialogSystem; // Ссылка на систему диалогов

    //Вспомогательные методы 
    std::string IntToString(int value);
    int GetScaledX(int x);
    int GetScaledY(int y);
    int GetScaledSize(int size);

    HDC buffer;

public:

    RenderSystem(ResourceManager& resManager)
        : r_resManager(resManager), buffer(nullptr), dialogSystem(nullptr) {
    }

    void SetBuffer(const HDC& memDC);
    void SetDialogSystem(DialogSystem* dialog) { dialogSystem = dialog; }

    void ShowText(HDC hdc, const std::string& utf8text, int base_x, int base_y, int base_font_size);
    void ShowANSIText(HDC hdc, const std::string& utf8text, int base_x, int base_y, int base_font_size);

    void ShowBMP(
        const HDC& hdc,
        int base_x, int base_y, 
        int base_w, int base_h,
        HBITMAP hBitmap, bool transparent = false, bool bitblt = false
    );

    void ShowObjectBeforeStart(float centrX = 1920.0f / 2.0f, float centrY = 1080.0f / 2.0f);
    void ShowProcessGame();
    void ShowDialog();
    void ShowTextNPC(int value, std::string text);
    void ShowAnswersNPC(int value, std::string text);


};