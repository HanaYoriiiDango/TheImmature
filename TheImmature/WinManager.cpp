#include "systems.h"
#include <iostream>

using namespace std;

// Реализации методов WindowManager 

bool WindowManager::RegistrClass(HINSTANCE hInstance) {
    if (!hInstance) return false;

    const wchar_t* CLASS_NAME = L"Main";
    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = NULL;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Error class registr!", L"Error", MB_ICONERROR);
        return false;
    }

    // Save:
    window.className = CLASS_NAME;
    window.hInstance = hInstance;
    return true;
}

bool WindowManager::WindowCreate() {

    window.width = GetSystemMetrics(SM_CXSCREEN);
    window.height = GetSystemMetrics(SM_CYSCREEN);

    window.hwnd = CreateWindowEx(0, window.className, L"The Immature", WS_POPUP | WS_MAXIMIZE, 0, 0, window.width, window.height, NULL, NULL, window.hInstance, NULL);

    if (!window.hwnd) return false;
    return true;

}

bool WindowManager::InitWindow() {

    if (!window.hwnd) return false; // Check HWND 

    RECT r;
    GetClientRect(window.hwnd, &r);

    window.width = r.right - r.left;
    window.height = r.bottom - r.top;

    const int BASE_WIDTH = 1920;
    const int BASE_HEIGHT = 1080;

    window.scale_x = (float)window.width / BASE_WIDTH;
    window.scale_y = (float)window.height / BASE_HEIGHT;

    window.ui_scale = min(window.scale_x, window.scale_y);

    return true;
}

void WindowManager::WinUpdate() {

    ShowWindow(window.hwnd, SW_SHOW);
    UpdateWindow(window.hwnd);

}

void WindowManager::WhileMessage() {

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
}