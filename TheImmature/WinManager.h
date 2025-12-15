// src/window/WindowManager.h
#pragma once
#include "Windows.h"
#include "Global.h"

class RenderSystem;
class ResourceManager;
class DialogSystem; 
class GameLogicSystem; 

class WindowManager {
private:

    RenderSystem& w_render;
    ResourceManager& w_resManager;
    DialogSystem* w_dialog; 
    GameLogicSystem* w_logic;

    // === Обработка сообщений ===
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool start = false; // флаг для начала отрисовки игры 

public:

    WindowManager(RenderSystem& render, ResourceManager& resManager,
        DialogSystem* dialog = nullptr, GameLogicSystem* logic = nullptr)
        : w_render(render), w_resManager(resManager),
        w_dialog(dialog), w_logic(logic) {
    }

    void SetDialogSystem(DialogSystem* dialog) { w_dialog = dialog; }
    void SetGameLogicSystem(GameLogicSystem* logic) { w_logic = logic; } // Сеттер

    // Настройка и создание окна
    bool WindowCreate();
    bool InitWindow();
    bool RegistrClass(HINSTANCE hInstance);
    void WinUpdate();
    void WhileMessage();

    // Остальное
    void Render();

};