#include "systems.h"
#include <iostream>

using namespace std;

// Реализации методов GameCore
bool GameCore::InitGame(HINSTANCE hInstance) {

    if(!Init.WindowInitialize(hInstance)) return false;
    if (!Init.BMPInitialize()) return false;

    //Init.CreateWorlds();
    //Manager.LoadAllNPCs();

    return true;
}

void GameCore::Update() {

    WinManager.WinUpdate();

}

void GameCore::Run() {

    WinManager.WhileMessage();


}
//void GameCore::StartGame() {
//
//    Collector.StartSession();
//
//}
//
//void GameCore::EndGame() {
//
//    Collector.EndSession();
//    Collector.SaveData();
//    Hero.life = false;
//
//}
//
//void GameCore::Help() {
//
//    cout << "help - список команд \n";
//    cout << "info - информация о инициализированных объектах \n";
//    cout << "status - информация о состоянии персонажа \n";
//    cout << "go - для перемещения \n";
//    cout << "start - начать диалог с персонажем (если есть с кем поболтать)\n";
//    cout << "exit - завершить игровую сессию\n";
//
//}
//
//void GameCore::ProcessCommand() {
//
//    cin >> temp;
//    if (temp == "help" || temp == "Help") Help();
//    if (temp == "status" || temp == "Status") Logic.StatusInfo();
//    if (temp == "info" || temp == "Info") Init.Info();
//    if (temp == "go" || temp == "Go") Logic.ProcessGo();
//    if (temp == "start" || temp == "Start") Dialog.ProcessDialog();
//    if (temp == "exit" || temp == "Exit") EndGame();
//    if (temp == "CLEAR23") Collector.ClearStatistics();
//
//}