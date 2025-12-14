#include "Global.h"
#include "Render.h"
#include "JsonManager.h" 

std::wstring RenderSystem::IntToWString(int value) {
    return std::to_wstring(value);
}

int RenderSystem::GetScaledX(int x, float scaleX) {

    return (int)(x * scaleX);

}
int RenderSystem::GetScaledY(int y, float scaleY) {

    return (int)(y * scaleY);

}
int RenderSystem::GetScaledSize(int size, float uiScale) {

    return (int)(size * uiScale);

}

void RenderSystem::ShowText(
    const HDC& hdc, 
    const std::wstring& text, 
    int base_x, 
    float windowScaleX,
    int base_y, 
    float windowScaleY,
    int base_font_size,
    float windowUiscale ) 
{

    int x = GetScaledX(base_x, windowScaleX);
    int y = GetScaledY(base_y, windowScaleY);
    int font_size = GetScaledSize(base_font_size, windowUiscale);
    font_size = max(12, font_size);

    HFONT hFont = CreateFontW(
        font_size, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, L"Arial"
    );

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    // drawing
    TextOutW(hdc, x, y, text.c_str(), (int)text.length());

    // clear
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);

}

void RenderSystem::ShowBMP(
    const HDC& hdc,
    int base_x, float windowScaleX,
    int base_y, float windowScaleY,
    float windowUiscale, int base_w, int base_h, 
    HBITMAP hBitmap, bool transparent) 
{

    BITMAP bm;
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    if (hBitmap) {

        int x = GetScaledX(base_x, windowScaleX);
        int y = GetScaledY(base_y, windowScaleY);
        int w = GetScaledSize(base_w, windowUiscale);
        int h = GetScaledSize(base_h, windowUiscale);

        GetObject(hBitmap, sizeof(BITMAP), &bm);

        if (transparent) TransparentBlt(hdc, x, y, w, h, memDC, 0, 0, w, h, RGB(0, 0, 0));
        else StretchBlt(hdc, x, y, w, h, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    }

    SelectObject(memDC, hOldBitmap);
    DeleteDC(memDC);

}

void RenderSystem::ShowObject(const HDC& hdc, float windowScaleX, float windowScaleY, float windowScaleUI) {

    if (!hdc) return;

    // background
    ShowBMP(hdc, 0, windowScaleX,  0, windowScaleY, windowScaleUI, 1920, 1080, r_resManager.GethBack(), false);

    // Back icon hero
    ShowBMP(hdc, 20, windowScaleX, 20, windowScaleY, windowScaleUI, 400, 600, r_resManager.GetBackCharacter(), true);

    //Back scales
    ShowBMP(hdc, 20, windowScaleX, 650, windowScaleY, windowScaleUI, 400, 400, r_resManager.GetBackScales(), true);

    //Back main text
    ShowBMP(hdc, 450, windowScaleX, 20, windowScaleY, windowScaleUI, 1000, 600, r_resManager.GetbackMainText(), true);

    //Back replaces
    ShowBMP(hdc, 450, windowScaleX, 650, windowScaleY, windowScaleUI, 1450, 400, r_resManager.GetBackReplace(), true);

    //Back icon character
    ShowBMP(hdc, 1500, windowScaleX, 20, windowScaleY, windowScaleUI, 400, 600, r_resManager.GetBackCharacter(), true);

    if (!r_jsonManager) {
        ShowText(hdc, L"JsonManager not set", 150, windowScaleX, 150, windowScaleY, 28, windowScaleUI);
        return;
    }

    // Используем данные из JsonManager
    for (int i = 0; i < COUNT_Emotions; i++) {
        Emotion_ emotion = static_cast<Emotion_>(i);
        std::wstring name = r_jsonManager->GetEmotionDisplayName(emotion);
        int value = 50;
        
        ShowText(hdc, name, 50, windowScaleX, 700 + i * 60, windowScaleY, 28, windowScaleUI);
        ShowText(hdc, IntToWString(value), 200, windowScaleX, 700 + i * 60, windowScaleY, 28, windowScaleUI);
    }
}
