#include "Global.h"
#include "Render.h"
#include "JsonManager.h" 
#include "Dialog.h" 

std::string RenderSystem::IntToString(int value) {
    return std::to_string(value);
}

int RenderSystem::GetScaledX(int x) {

    return (int)(x * window.scaleX);

}
int RenderSystem::GetScaledY(int y) {

    return (int)(y * window.scaleY);

}
int RenderSystem::GetScaledSize(int size) {

    return (int)(size * window.uiScale);

}
void RenderSystem::SetBuffer(const HDC& memDC) { buffer = memDC; }

void RenderSystem::ShowText(
    const HDC& hdc,
    const std::string& text,
    int base_x,
    int base_y,
    int base_font_size
)
{
    int x = GetScaledX(base_x);
    int y = GetScaledY(base_y);
    int font_size = GetScaledSize(base_font_size);
    font_size = max(12, font_size);

    HFONT hFont = CreateFontA(
        font_size, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, "Arial"
    );

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);

    // === ИЗМЕНЕНИЕ НАЧИНАЕТСЯ ЗДЕСЬ ===

    // Разделяем текст по \n
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end = text.find('\n');

    while (end != std::string::npos) {
        lines.push_back(text.substr(start, end - start));
        start = end + 1;
        end = text.find('\n', start);
    }
    lines.push_back(text.substr(start));

    // Рисуем каждую строку
    int lineHeight = font_size + 4; // Межстрочный интервал
    for (size_t i = 0; i < lines.size(); i++) {
        TextOutA(hdc, x, y + (i * lineHeight),
            lines[i].c_str(), (int)lines[i].length());
    }

    // === ИЗМЕНЕНИЕ ЗАКОНЧИЛОСЬ ===

    // clear
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void RenderSystem::ShowBMP(
    const HDC& hdc,
    int base_x, int base_y, 
    int base_w, int base_h, 
    HBITMAP hBitmap, bool transparent, bool bitblt) 
{

    BITMAP bm;
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    if (hBitmap) {

        int x = GetScaledX(base_x);
        int y = GetScaledY(base_y);
        int w = GetScaledSize(base_w);
        int h = GetScaledSize(base_h);

        GetObject(hBitmap, sizeof(BITMAP), &bm);

        if (transparent) TransparentBlt(hdc, x, y, w, h, memDC, 0, 0, w, h, RGB(0, 0, 0));
        if (bitblt) BitBlt(hdc, x, y, w, h, memDC, 0, 0, SRCCOPY);
        else StretchBlt(hdc, x, y, w, h, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    }

    SelectObject(memDC, hOldBitmap);
    DeleteDC(memDC);

}

void RenderSystem::ShowObjectBeforeStart(float centrX, float centrY) {

    if (!buffer) return;

    // background
    ShowBMP(buffer, 0, 0, 1920, 1080, r_resManager.GetbackMainText());

    ShowText(buffer, "Press SPACE to start", centrX, centrY, 56);

}

void RenderSystem::ShowProcessGame() {

    if (!buffer) return;

    // 1. Фон в зависимости от текущего мира
    HBITMAP currentBg = Worlds[Hero.current_loc].background;
    if (!currentBg) currentBg = r_resManager.GethBack(); // fallback

    ShowBMP(buffer, 0, 0, 1920, 1080, currentBg, false);

    // Back icon hero
    ShowBMP(buffer, 20, 20, 400, 600, r_resManager.GetBackCharacter(), false, true);

    //Back scales
    ShowBMP(buffer, 20, 650, 400, 400, r_resManager.GetBackScales(), false, true);

    //Back main text
    ShowBMP(buffer, 450, 20, 1020, 600, r_resManager.GetbackMainText(), false, true);

    //Back replaces
    ShowBMP(buffer, 450, 650, 1440, 400, r_resManager.GetBackReplace(), false, true);

    //Back icon character
    ShowBMP(buffer, 1500, 20, 390, 600, r_resManager.GetBackCharacter(), false, true);

    // Выводим эмоции
    for (int i = 0; i < Emotion.size(); i++) {

        ShowText(buffer, Emotion_Names[i], 30, 700 + i * 60, 28);
        ShowText(buffer, std::to_string(Hero.emotions[i]), 200, 700 + i * 60, 28);

    }

    // Выводи текущий мир 
    ShowText(buffer, Worlds[Hero.current_loc].name, 30, 660, 28);

    Hero.icon_show = true;

    if (Hero.Icon) {
        if (Hero.icon_show) {

            ShowBMP(buffer, 40, 40, 370, 560, Hero.Icon, false, true);

        }
    }

    // Если диалог активен - рендерим его
    if (game.Current_State == DIALOG && dialogSystem) {
        dialogSystem->Render(buffer);
    }

    


}

void RenderSystem::ShowTextNPC(int value, std::string text) {

    ShowText(buffer, text, 470, 40 + value, 28);

}

void RenderSystem::ShowAnswersNPC(int value, std::string text) {

    ShowText(buffer, text, 470, 660 + value, 28);

}