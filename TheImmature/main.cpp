// options: 
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) 
//configuration::advanced::character set - use unicode character set 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "Global.h"
#include "Systems.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow) 
{
    GameCore Core;

    if (!Core.InitGame(hInstance)) return 0;
    Core.Update();
    Core.Run();
  
}
