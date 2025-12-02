// Настройки: 
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) 
//configuration::advanced::character set - use unicode character set 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include <windows.h>
#include <iostream>

using namespace std;

enum Emotion_ { JOY, SADNESS, POWER, FEAR, CALM, ANGER, COUNT_Emotions };


struct {
	int width, height;
    HWND hwnd;
	HBITMAP hBack;
    HDC hdc, mem_dc;


}window;

struct Player {
    int current_loc = SADNESS;
    int emotions[COUNT_Emotions] = { 50, 50, 50, 50, 50, 50 };
    bool life = true;
};

void InitWindow() {

    RECT r;
    GetClientRect(window.hwnd, &r);

    window.width = r.right - r.left;
    window.height = r.bottom - r.top;

}

bool FindFiles(const wchar_t* filename) { // ищет файл в проекте
    // true - найден, false - не найден
    return GetFileAttributesW(filename) != INVALID_FILE_ATTRIBUTES;
}

HBITMAP LoadBMP(const wchar_t* name) {

    // сначало проверяем существует ли такой файл в проекте  
    if (!FindFiles(name)) { 
        
        MessageBoxW(NULL, L"Файл в проекте не найден", L"Ошибка", MB_ICONERROR);
        return NULL;
    }

    // если он есть, то пробуем загружать файл
    HBITMAP hBmp = (HBITMAP)LoadImageW(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // проверяем загрузился ли файл
    if (!hBmp) MessageBox(NULL, L"Ошибка загрузки файла", L"Ошибка", MB_ICONERROR);
    return hBmp; // если да, то возвращаем

}

void ShowText(const wchar_t* text, int size, int x, int y) {

    // Настройки текста: 
    SetTextColor(window.mem_dc, RGB(0, 0, 0)); // черный
    SetBkMode(window.mem_dc, TRANSPARENT);            // Прозрачный фон
    TextOutW(window.mem_dc, x, y, text, size); // положение, какой текст вывести и размер

}

void InitGame() {

    window.hBack = LoadBMP(L"WhileBack.bmp");


}

void ShowSprite(int x, int y, int w, int h, HBITMAP hBitmap, bool transparent) {

    BITMAP bm;
    HDC memDC = CreateCompatibleDC(window.mem_dc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    if (hBitmap) {

        GetObject(hBitmap, sizeof(BITMAP), &bm);

        if (transparent) TransparentBlt(window.mem_dc, x, y, w, h, memDC, 0, 0, w, h, RGB(0, 0, 0));
        else StretchBlt(window.mem_dc, x, y, w, h, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    }

    SelectObject(memDC, hOldBitmap);
    DeleteDC(memDC);

}

void ShowObject() {

    //// Вместо сложной отрисовки - просто закрашиваем фон
    //HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 100));
    //RECT rc = { 0, 0, window.width, window.height };
    //FillRect(window.mem_dc, &rc, hBrush);
    //DeleteObject(hBrush);  // ← Важно!


    ShowSprite(0, 0, window.width, window.height, window.hBack, false);

}
void ShowGame() {

    window.mem_dc = CreateCompatibleDC(window.hdc);
    HBITMAP hMemBmp = CreateCompatibleBitmap(window.hdc, window.width, window.height);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(window.mem_dc, hMemBmp);

    ShowObject();
    ShowText(L"GFGFGFGF", 8, 100, 100);

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
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 0;
    }

    window.width = GetSystemMetrics(SM_CXSCREEN); 
    window.height = GetSystemMetrics(SM_CYSCREEN);

    window.hwnd = CreateWindowEx(0, CLASS_NAME, L"The Immature", WS_POPUP | WS_MAXIMIZE, 0, 0, window.width, window.height, NULL, NULL, hInstance, NULL);

    if (!window.hwnd) {  
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
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
        if (wParam == VK_ESCAPE) DestroyWindow(window.hwnd); // уничтожаем окно

        break;

    case WM_DESTROY:

        PostQuitMessage(0);
        break;

    default: return DefWindowProc(hwnd, msg, wParam, lParam);

    }
}


