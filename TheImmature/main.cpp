// ?????????: 
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) 
//configuration::advanced::character set - use unicode character set 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

enum Emotion_ { JOY, SADNESS, POWER, FEAR, CALM, ANGER, COUNT_Emotions };
vector<Emotion_> Emotion{ JOY, SADNESS, POWER, FEAR, CALM, ANGER };

wstring Emotion_Names[COUNT_Emotions] = { L"JOY", L"SADNESS", L"POWER", L"FEAR", L"CALM", L"ANGER" };


struct {
	int width, height;
    float scale_x, scale_y;
    float ui_scale; // Единый масштаб для всего

    HWND hwnd;
	HBITMAP hBack;
    HDC hdc, mem_dc;

    HBITMAP BackScales;
    HBITMAP BackReplace;
    HBITMAP BackHero;
    HBITMAP BackCharacter;
    HBITMAP BackMainText;

}window;

struct Player {
    int current_loc = SADNESS;
    int emotions[COUNT_Emotions] = { 50, 50, 50, 50, 50, 50 };
    bool life = true;
};

Player Hero;

void InitWindow() {

    RECT r;
    GetClientRect(window.hwnd, &r);

    window.width = r.right - r.left;
    window.height = r.bottom - r.top;

    const int BASE_WIDTH = 1920;
    const int BASE_HEIGHT = 1080;

    window.scale_x = (float)window.width / BASE_WIDTH;
    window.scale_y = (float)window.height / BASE_HEIGHT;

    window.ui_scale = min(window.scale_x, window.scale_y);

}

wstring IntToWString(int value) {
    return to_wstring(value);
}

int GetScaledX(int x) { return (int)(x * window.scale_x); }
int GetScaledY(int y) { return (int)(y * window.scale_y); }
int GetScaledSize(int size) { return (int)(size * window.ui_scale); }

bool FindFiles(const wchar_t* filename) { 
    
    return GetFileAttributesW(filename) != INVALID_FILE_ATTRIBUTES;
}

HBITMAP LoadBMP(const wchar_t* name) {

     
    if (!FindFiles(name)) { 
        
        MessageBoxW(NULL, L"No find files", L"sosi", MB_ICONERROR);
        return NULL;
    }

    
    HBITMAP hBmp = (HBITMAP)LoadImageW(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    
    if (!hBmp) MessageBox(NULL, L"No find bmp", L"sosi", MB_ICONERROR);
    return hBmp; 

}

void ShowText(const wstring& text, int x, int y, int base_font_size = 24) {

    int scaled_size = GetScaledSize(base_font_size);
    scaled_size = max(12, scaled_size); // Min 12px

    HFONT hFont = CreateFontW(scaled_size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    HFONT hOldFont = (HFONT)SelectObject(window.mem_dc, hFont);

    SetTextColor(window.mem_dc, RGB(0, 0, 0));
    SetBkMode(window.mem_dc, TRANSPARENT);
    TextOutW(window.mem_dc, x, y, text.c_str(), text.length());

    SelectObject(window.mem_dc, hOldFont);
    DeleteObject(hFont);

}

void InitGame() {

    window.hBack = LoadBMP(L"SadBack.bmp");
    window.BackScales = LoadBMP(L"WhiteBack.bmp");
    window.BackReplace = LoadBMP(L"WhiteBack.bmp");
    window.BackMainText = LoadBMP(L"WhiteBack.bmp");
    window.BackCharacter = LoadBMP(L"WhiteBack.bmp");
    window.BackHero = LoadBMP(L"WhiteBack.bmp");

}

void ShowBMP(int base_x, int base_y, int base_w, int base_h, HBITMAP hBitmap, bool transparent = false) {

    BITMAP bm;
    HDC memDC = CreateCompatibleDC(window.mem_dc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    if (hBitmap) {

        int x = GetScaledX(base_x);
        int y = GetScaledY(base_y);
        int w = GetScaledSize(base_w);
        int h = GetScaledSize(base_h);

        GetObject(hBitmap, sizeof(BITMAP), &bm);

        if (transparent) TransparentBlt(window.mem_dc, x, y, w, h, memDC, 0, 0, w, h, RGB(0, 0, 0));
        else StretchBlt(window.mem_dc, x, y, w, h, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    }

    SelectObject(memDC, hOldBitmap);
    DeleteDC(memDC);

}

void ShowObject() {

    // background
    ShowBMP(0, 0, 1920, 1080, window.hBack, false);

    // Back icon hero
    ShowBMP(20, 20, 400, 600, window.BackHero, true);

    //Back scales
    ShowBMP(20, 650, 400, 400, window.BackScales, true);

    //Back main text
    ShowBMP(450, 20, 1000, 600, window.BackMainText, true);

    //Back replaces
    ShowBMP(450, 650, 1450, 400, window.BackReplace, true);

    //Back icon character
    ShowBMP(1500, 20, 400, 600, window.BackCharacter, true);

    // scales:
    for (int i = 0; i < COUNT_Emotions; i++) {
        ShowText(Emotion_Names[i], 50, 900 + i * 60, 28);
        ShowText(to_wstring(Hero.emotions[i]), 200, 900 + i * 60, 28);
    }
}

void ShowGame() {

    window.mem_dc = CreateCompatibleDC(window.hdc);
    HBITMAP hMemBmp = CreateCompatibleBitmap(window.hdc, window.width, window.height);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(window.mem_dc, hMemBmp);

    ShowObject();

    BitBlt(window.hdc, 0, 0, window.width, window.height, window.mem_dc, 0, 0, SRCCOPY);

    SelectObject(window.mem_dc, hMemBmp);
    DeleteObject(hMemBmp);
    DeleteDC(window.mem_dc);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow ) {

    const wchar_t* CLASS_NAME = L"Main";  
    WNDCLASSEX wc = {}; 

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc; 
    wc.hInstance = hInstance; 
    wc.hCursor = NULL; 
    wc.lpszClassName = CLASS_NAME;  

    if (!RegisterClassEx(&wc)) { 
        MessageBox(NULL, L"Error class registr!", L"sosi", MB_ICONERROR);
        return 0;
    }

    window.width = GetSystemMetrics(SM_CXSCREEN); 
    window.height = GetSystemMetrics(SM_CYSCREEN);

    window.hwnd = CreateWindowEx(0, CLASS_NAME, L"The Immature", WS_POPUP | WS_MAXIMIZE, 0, 0, window.width, window.height, NULL, NULL, hInstance, NULL);

    if (!window.hwnd) {  
        MessageBox(NULL, L"NULL hwnd", L"sosi", MB_ICONERROR);
        return 0; 

    }

    InitWindow();

    ShowWindow(window.hwnd, SW_SHOW);
    UpdateWindow(window.hwnd);

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_CREATE:
        InitGame();

        break;
    
    case WM_PAINT: 
    {
        PAINTSTRUCT ps;
        window.hdc = BeginPaint(hwnd, &ps);

        ShowGame();
        EndPaint(hwnd, &ps);

        break;
    }

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) DestroyWindow(window.hwnd); 

        break;

    case WM_DESTROY:

        PostQuitMessage(0);
        break;

    default: return DefWindowProc(hwnd, msg, wParam, lParam);

    }
}


