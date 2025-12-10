// options: 
//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS) 
//configuration::advanced::character set - use unicode character set 
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include "json.hpp"
#include "Global.h" 
#include "Core.h"
#include <cstdlib>
#include <ctime>

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow) 
{
    GameCore Core;

    if (!Core.InitGame(hInstance)) return 0;
    Core.Run();
  
}
