// src/core/GameCore.cpp
#include "Core.h"

GameCore::GameCore()
    : c_Render(c_ResManager)
    , c_WinManager(c_Render, c_ResManager)
    , c_Init(c_WinManager, c_ResManager, c_JsonManager) 
    , c_Logic()  
{
}

bool GameCore::InitGame(HINSTANCE hInstance) {

    if(!c_Init.WindowInitialize(hInstance)) return false;
    if (!c_Init.BMPInitialize()) return false;

    //Init.CreateWorlds();
    //Manager.LoadAllNPCs();

    // ЗАГРУЗИТЬ МАНИФЕСТ
    if (!c_Init.ManifestInitialize()) {
        MessageBox(NULL, L"Ошибка загрузки манифеста", L"Ошибка", MB_OK);
        return false;
    }

    c_Render.SetJsonManager(&c_JsonManager);

    // ЗАГРУЗИТЬ NPC (НЕ критично для MVP)
    if (!c_JsonManager.LoadAllNPCs()) {
        // Только предупреждение
        JsonValidator::LogInfo("Core", "No NPCs loaded, continuing anyway");
    }
    else {
        // Проверяем что загрузилось
        auto& npcs = c_JsonManager.GetNPCs();  // ← Теперь работает!
        JsonValidator::LogInfo("Core",
            "Successfully loaded " +
            std::to_string(npcs.size()) + " NPCs");

        // Можно проверить первого NPC для отладки
        if (!npcs.empty()) {
            const NPC& firstNPC = npcs[0];
            std::wstring debugMsg = L"First NPC: " + firstNPC.name +
                L" in world: " +
                c_JsonManager.GetEmotionDisplayName(firstNPC.world_link);
            OutputDebugStringW((debugMsg + L"\n").c_str());
        }
    }

    return true;
}

void GameCore::Run() {

    c_WinManager.WinUpdate();
    c_WinManager.WhileMessage();

}

////void GameCore::StartGame() {
////
////    Collector.StartSession();
////
////}
////
////void GameCore::EndGame() {
////
////    Collector.EndSession();
////    Collector.SaveData();
////    Hero.life = false;
////
////}
////
////void GameCore::Help() {
////
////    cout << "help - список команд \n";
////    cout << "info - информация о инициализированных объектах \n";
////    cout << "status - информация о состоянии персонажа \n";
////    cout << "go - для перемещения \n";
////    cout << "start - начать диалог с персонажем (если есть с кем поболтать)\n";
////    cout << "exit - завершить игровую сессию\n";
////
////}
////
////void GameCore::ProcessCommand() {
////
////    cin >> temp;
////    if (temp == "help" || temp == "Help") Help();
////    if (temp == "status" || temp == "Status") Logic.StatusInfo();
////    if (temp == "info" || temp == "Info") Init.Info();
////    if (temp == "go" || temp == "Go") Logic.ProcessGo();
////    if (temp == "start" || temp == "Start") Dialog.ProcessDialog();
////    if (temp == "exit" || temp == "Exit") EndGame();
////    if (temp == "CLEAR23") Collector.ClearStatistics();
////
////}