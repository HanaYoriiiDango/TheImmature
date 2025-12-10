#pragma once
#include "ResManager.h"
#include "Global.h"

class ManifestManager;

// src/render/RenderSystem.h

class RenderSystem {
private:
    ResourceManager& r_resManager;
    const ManifestManager* r_manifestManager = nullptr;


    //Вспомогательные методы 
    std::wstring IntToWString(int value);
    int GetScaledX(int x, float scaleX);
    int GetScaledY(int y, float scaleY);
    int GetScaledSize(int size, float uiScale);

public:

    RenderSystem(ResourceManager& rm) : r_resManager(rm) {}

    // Установить указатель на ManifestManager
    void SetManifestManager(const ManifestManager* mm) {
        r_manifestManager = mm;
    }


     void ShowText(
        const HDC& hdc, const std::wstring& text,
        int base_x, float windowScaleX,
        int base_y, float windowScaleY,
        int base_font_size, float windowUiscale
    );

    void ShowBMP(
        const HDC& hdc,
        int base_x, float windowScaleX,
        int base_y, float windowScaleY,
        float windowUiscale, int base_w, int base_h,
        HBITMAP hBitmap, bool transparent = false
    );

    void ShowObject(const HDC& hdc, float windowScaleX, float windowScaleY, float windowScaleUI);

};