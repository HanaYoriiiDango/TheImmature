// src/core/GameCore.cpp
#include "Core.h"

GameCore::GameCore()
    : c_Render(c_ResManager)           // RenderSystem получает ResourceManager
    , c_WinManager(c_Render, c_ResManager)  // WindowManager получает оба
    , c_Init(c_WinManager, c_ResManager, c_Validator)    // InitSystem получает оба
    , c_Validator()                      // JsonManager без параметров
{}

bool GameCore::InitGame(HINSTANCE hInstance) {

    if(!c_Init.WindowInitialize(hInstance)) return false;
    if (!c_Init.BMPInitialize()) return false;

    //Init.CreateWorlds();
    //Manager.LoadAllNPCs();
    // 
    // ЗАГРУЗИТЬ МАНИФЕСТ
    if (!c_Init.ManifestInitialize()) {
        MessageBox(NULL, L"Ошибка загрузки манифеста", L"Ошибка", MB_OK);
        return false;
    }

    // ПЕРЕДАТЬ ManifestManager в RenderSystem (передаём АДРЕС)
    c_Render.SetManifestManager(&c_Manifest); // & - получаем адрес объекта

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