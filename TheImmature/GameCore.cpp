// src/core/GameCore.cpp
#include "Core.h"

GameCore::GameCore()
    : Render(ResManager)
    , WinManager(Render, ResManager, &Dialog, &Logic) 
    , Init(WinManager, ResManager, JsonManager)
    , Logic()
    , JsonManager(ResManager)
    , Dialog(JsonManager, Logic, Render)
    , JsonValidator
{
    // Уже связали через конструктор, но можно и явно:
    WinManager.SetGameLogicSystem(&Logic);
}

bool GameCore::InitGame(HINSTANCE hInstance) {

    if(!Init.WindowInitialize(hInstance)) return false;
    if (!Init.BMPInitialize()) return false;

    Init.CreateWorlds();
    //Manager.LoadAllNPCs();

    //c_Render.SetJsonManager(&c_JsonManager);

    // Устанавливаем связь между Render и Dialog
    Render.SetDialogSystem(&Dialog);
    
    Logic.SetRenderSystem(&Render);

    // ЗАГРУЗИТЬ NPC (НЕ критично для MVP)
    if (!JsonManager.LoadAllNPCs()) {
        // Только предупреждение
        JsonValidator::LogInfo("Core", "No NPCs loaded, continuing anyway");
    }

    return true;
}

void GameCore::Run() {

    WinManager.WinUpdate();
    WinManager.WhileMessage();

}

//// вот эти функции пересмотреть на формат 2D
// void GameCore::StartGame() {
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