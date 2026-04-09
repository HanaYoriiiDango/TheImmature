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

void RenderSystem::ShowText(HDC hdc, const std::string& utf8text,
    int base_x, int base_y, int base_font_size)
{
    int x = GetScaledX(base_x);
    int y = GetScaledY(base_y);
    int font_size = GetScaledSize(base_font_size);
    font_size = max(12, font_size);

    // UTF-8 → UTF-16
    int wstrSize = MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(), -1, NULL, 0);
    std::wstring wstr(wstrSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(), -1, &wstr[0], wstrSize);

    // Создаем шрифт (CreateFontW для Unicode)
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

    // Разделяем по \n (работает одинаково для всех строк)
    std::vector<std::wstring> lines;
    size_t start = 0;
    size_t end = utf8text.find('\n');
    std::string temp = utf8text;

    while (end != std::string::npos) {
        std::string line = temp.substr(start, end - start);
        // Конвертируем каждую строку отдельно
        int sz = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, NULL, 0);
        std::wstring wline(sz, 0);
        MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, &wline[0], sz);
        lines.push_back(wline);

        start = end + 1;
        end = temp.find('\n', start);
    }
    // Последняя строка
    std::string lastLine = temp.substr(start);
    int szLast = MultiByteToWideChar(CP_UTF8, 0, lastLine.c_str(), -1, NULL, 0);
    std::wstring wlast(szLast, 0);
    MultiByteToWideChar(CP_UTF8, 0, lastLine.c_str(), -1, &wlast[0], szLast);
    lines.push_back(wlast);

    // Рисуем через TextOutW
    int lineHeight = font_size + 4;
    for (size_t i = 0; i < lines.size(); i++) {
        TextOutW(hdc, x, y + i * lineHeight,
            lines[i].c_str(), lines[i].length());
    }

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void RenderSystem::ShowANSIText(HDC hdc, const std::string& ansiText,
    int base_x, int base_y, int base_font_size)
{
    int x = GetScaledX(base_x);
    int y = GetScaledY(base_y);
    int font_size = GetScaledSize(base_font_size);
    font_size = max(12, font_size);

    // Вся строка целиком (для CreateFontW не нужно, но оставим)
    int size = MultiByteToWideChar(1251, 0, ansiText.c_str(), -1, NULL, 0);
    std::wstring wstr(size, 0);
    MultiByteToWideChar(1251, 0, ansiText.c_str(), -1, &wstr[0], size);

    // Создаем шрифт
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

    // Разделяем по \n
    std::vector<std::wstring> lines;
    size_t start = 0;
    size_t end = ansiText.find('\n');
    std::string temp = ansiText;

    while (end != std::string::npos) {
        std::string line = temp.substr(start, end - start);
        // КОНВЕРТИРУЕМ С 1251, А НЕ CP_UTF8!
        int sz = MultiByteToWideChar(1251, 0, line.c_str(), -1, NULL, 0);  // ← ИСПРАВЛЕНО
        std::wstring wline(sz, 0);
        MultiByteToWideChar(1251, 0, line.c_str(), -1, &wline[0], sz);
        lines.push_back(wline);

        start = end + 1;
        end = temp.find('\n', start);
    }

    // Последняя строка
    std::string lastLine = temp.substr(start);
    int szLast = MultiByteToWideChar(1251, 0, lastLine.c_str(), -1, NULL, 0);  // ← ИСПРАВЛЕНО
    std::wstring wlast(szLast, 0);
    MultiByteToWideChar(1251, 0, lastLine.c_str(), -1, &wlast[0], szLast);
    lines.push_back(wlast);

    // Рисуем через TextOutW
    int lineHeight = font_size + 4;
    for (size_t i = 0; i < lines.size(); i++) {
        TextOutW(hdc, x, y + i * lineHeight,
            lines[i].c_str(), lines[i].length());
    }

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

        if (transparent) TransparentBlt(hdc, x, y, w, h, memDC, 0, 0, w, h, RGB(255, 255, 255));
        if (bitblt) BitBlt(hdc, x, y, w, h, memDC, 0, 0, SRCCOPY);
        else StretchBlt(hdc, x, y, w, h, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    }

    SelectObject(memDC, hOldBitmap);
    DeleteDC(memDC);

}

void RenderSystem::ShowObjectBeforeStart(float centrX, float centrY) {

    if (!buffer) return;

    // background
    ShowBMP(buffer, 0, 0, 1920, 1080, Interface.hBack);

    ShowText(buffer, "Press SPACE to start", centrX, centrY, 56);

}

void RenderSystem::DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) { // algorithm Brezenheim
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        SetPixel(hdc, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }

}

void RenderSystem::PixelDiagramm() {

    if (!buffer) return;

    int cx = GetScaledX(218);   
    int cy = GetScaledY(848);   
    int maxR = GetScaledSize(160);  

    int pointsX[6], pointsY[6];

    for (int i = 0; i < 6; i++) {
        float angle = (3.14159f / 2.0f) - i * (2.0f * 3.14159f / 6.0f);
        float t = Hero.emotions[i] / 100.0f;
        float r = maxR * t;

        pointsX[i] = cx + (int)(r * cos(angle));
        pointsY[i] = cy - (int)(r * sin(angle));




    }

    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        DrawLine(buffer, pointsX[i], pointsY[i], pointsX[next], pointsY[next], RGB(255, 0, 255));
    }

}

void RenderSystem::ShowProcessGame() {

    if (!buffer) return;

    HBITMAP currentBg = Worlds[Hero.current_loc].background;
    if (!currentBg) currentBg = Interface.hBack; // fallback

    ShowBMP(buffer, 0, 0, 1920, 1080, currentBg, false);

    // Back icon hero
    ShowBMP(buffer, 20, 20, 400, 600, Interface.backCharacter, true, false);

    //Back scales
    ShowBMP(buffer, 20, 650, 400, 400, Interface.backScales, true, false);

    //Back main text
    ShowBMP(buffer, 450, 20, 1020, 600, Interface.backMainText, true, false);

    //Back replaces
    ShowBMP(buffer, 450, 650, 1200, 450, Interface.backReplace, false, true);

    //Back icon character
    ShowBMP(buffer, 1500, 20, 390, 600, Interface.backCharacter, true, false);

    PixelDiagramm();

    // output current world 
    ShowANSIText(buffer, Worlds[Hero.current_loc].name, 30, 660, 28);

    Hero.icon_show = true;

    if (Hero.Icon) {
        if (Hero.icon_show) {

            //ShowBMP(buffer, 25, 25, 400, 400, Hero.Icon, true);

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