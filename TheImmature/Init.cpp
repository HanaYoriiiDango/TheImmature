#include "systems.h"
#include <iostream>

using namespace std;

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