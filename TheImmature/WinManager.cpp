#include "WinManager.h" 
#include "Render.h" 
#include "Dialog.h" 

// Реализации методов WindowManager 

LRESULT CALLBACK WindowManager::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Обрабатываем WM_NCCREATE - первое сообщение при создании окна
    if (msg == WM_NCCREATE) {
        // 1. Получаем структуру создания окна
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;

        // 2. В pCreate->lpCreateParams лежит наш this!
        // Мы передали его в CreateWindowEx
        WindowManager* pThis = (WindowManager*)pCreate->lpCreateParams;

        // 3. Сохраняем указатель в "кармашек" окна
        if (pThis) {
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            // 4. Также сохраняем HWND в нашу структуру window
            window.hwnd = hwnd;
        }
    }

    // 5. Для ВСЕХ сообщений (включая WM_NCCREATE) пытаемся достать указатель
    WindowManager* pThis = (WindowManager*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    // 6. Если нашли наш объект - вызываем его метод
    if (pThis) {
        return pThis->HandleMessage(hwnd, msg, wParam, lParam);
    }

    // 7. Если не нашли - стандартная обработка
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT WindowManager::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        SetTimer(hwnd, 1, 16, NULL);

        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        window.hdc = BeginPaint(hwnd, &ps);

        Render();

        EndPaint(hwnd, &ps);

    }
                 break;

    case WM_TIMER:
    {

        // ЕДИНСТВЕННОЕ место для автозапуска
        if (g_NeedAutoStartDialog && w_dialog && game.Current_State == DIALOG) {
            OutputDebugStringA("[WM TIMER] Запуск диалога...");
            w_dialog->StartDialogInWorld();
            g_NeedAutoStartDialog = false;
        }

        InvalidateRect(hwnd, NULL, FALSE);

    }

    break;


    case WM_KEYDOWN:
        if (wParam == VK_SPACE && !start) {
            start = true;
            game.Current_State = DIALOG;
            g_NeedAutoStartDialog = true; // ТОЛЬКО ФЛАГ, таймер сам запустит
            return 0;
        }

        if (start) {
            // Обрабатываем в зависимости от состояния игры
            if (game.Current_State == DIALOG && w_dialog) {
                w_dialog->ProcessInput((int)wParam);
            }
            else if (game.Current_State == WORLD_SELECTION && w_logic) {
                w_logic->ProcessWorldSelection((int)wParam);
            }

            // Уже закомментировано - отлично!
            // if (g_NeedAutoStartDialog && w_dialog) {
            //     w_dialog->StartDialogInWorld();
            //     g_NeedAutoStartDialog = false;
            // }

            InvalidateRect(hwnd, NULL, TRUE);
        }

        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
            return 0;
        }
        break;
    case WM_DESTROY:
        g_NeedAutoStartDialog = false;
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;

    default:
        break;
    }

    // Все остальные сообщения передаем стандартной обработке
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void WindowManager::Render() {

    // Проверяем существование HDC  
    if (!window.hdc) {
        MessageBoxA(NULL, "Render Error", "window HDC is NULL", MB_ICONERROR);
        return;
    }

    // Создаем буфер для двойной буферизации
    HDC memDC = CreateCompatibleDC(window.hdc);

    if (!memDC) {
        MessageBoxA(NULL, "Render Error", "Failed to CreateCompatibleDC", MB_ICONERROR);
        return;
    }

    // Создаем битмап 
    HBITMAP hMemBmp = CreateCompatibleBitmap(window.hdc, window.width, window.height);

    if (!hMemBmp) {
        MessageBoxA(NULL, "Render Error", "Failed to CreateCompatibleBitmap", MB_ICONERROR);
        DeleteDC(memDC);
        return;
    }

    // Выбираем битмап в контекст 
    HBITMAP hOldBmp = (HBITMAP)SelectObject(memDC, hMemBmp);

    w_render.SetBuffer(memDC);

    // Вызываем RenderSystem и передаем ВСЕ необходимые данные
    w_render.ShowObjectBeforeStart();

    w_render.SetBuffer(memDC);

    // Вызываем RenderSystem
    if (!start) {
        w_render.ShowObjectBeforeStart();
    }
    else {
        w_render.ShowProcessGame(); // Это рисует фон, UI и т.д.

        if (g_NeedAutoStartDialog && w_dialog) {
            OutputDebugStringA("[WM RENDER] Автозапуск диалога!");
            //w_dialog->StartDialogInWorld();
            //g_NeedAutoStartDialog = false;
        }

        // ⭐⭐⭐ НОВЫЙ КОД: рендерим поверх в зависимости от состояния ⭐⭐⭐
        if (game.Current_State == DIALOG && w_dialog) {
            w_dialog->Render(memDC);
        }
        else if (game.Current_State == WORLD_SELECTION && w_logic) {
            // Рендерим выбор мира через GameLogicSystem
            w_logic->RenderWorldSelection(memDC);
        }
    }

    // Копируем буфер на экран
    BitBlt(window.hdc, 0, 0, window.width, window.height, memDC, 0, 0, SRCCOPY);

    // Очистка
    SelectObject(memDC, hOldBmp);  // Восстанавливаем старый битмап
    DeleteObject(hMemBmp);         // Удаляем наш битмап
    DeleteDC(memDC);               // Удаляем контекст
}

bool WindowManager::RegistrClass(HINSTANCE hInstance) {
    if (!hInstance) return false;

    const char* CLASS_NAME = "Main";
    WNDCLASSEXA  wc = {};

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.lpfnWndProc = StaticWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = NULL;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassExA(&wc)) {
        MessageBoxA(NULL, "Error class registr!", "Error", MB_ICONERROR);
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

    window.hwnd = CreateWindowExA(
        0,
        window.className,
        "The Immature",
        WS_POPUP | WS_MAXIMIZE,
        0, 0,
        window.width, window.height,
        NULL, NULL, window.hInstance, this);

    if (!window.hwnd) return false;

    SetCursor(NULL); // Скрыть курсор
    ShowCursor(FALSE);

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

    window.scaleX = (float)window.width / BASE_WIDTH;
    window.scaleY = (float)window.height / BASE_HEIGHT;

    window.uiScale = min(window.scaleX, window.scaleY);

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