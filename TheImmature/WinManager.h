// src/window/WindowManager.h
#pragma once
#include "Windows.h"

class RenderSystem;
class ResourceManager;

class WindowManager {
private:

    struct {

        // === Основные поля окна ===
        HWND hwnd = nullptr;
        HINSTANCE hInstance = nullptr;
        const wchar_t* className = L"Main";

        // === Размеры и масштабирование ===
        int width = 0;
        int height = 0;
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        float uiScale = 1.0f;

        // === Контексты устройств ===
        HDC hdc = nullptr, memDC = nullptr;   // Основной HDC и буфер 



    }window;

    RenderSystem& w_render;
    ResourceManager& w_resManager;

    // === Обработка сообщений ===
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


public:

    WindowManager(RenderSystem& rs, ResourceManager& rm) : w_render(rs), w_resManager(rm) {}

    // Настройка и создание окна
    bool WindowCreate();
    bool InitWindow();
    bool RegistrClass(HINSTANCE hInstance);
    void WinUpdate();
    void WhileMessage();

    // Остальное
    void Render();

};