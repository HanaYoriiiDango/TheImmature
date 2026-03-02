#include "ResManager.h" 
#include "filesystem"
#include <commdlg.h>




bool ResourceManager::FindFiles(const wchar_t* filename) {

    return GetFileAttributesW(filename) != INVALID_FILE_ATTRIBUTES;
}

HBITMAP ResourceManager::LoadBMP(const wchar_t* name) {
    if (!FindFiles(name)) {
        MessageBoxW(NULL, L"No find files", L"sosi", MB_ICONERROR);
        return NULL;
    }

    HBITMAP hBmp = (HBITMAP)LoadImageW(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmp) {
        MessageBoxW(NULL, L"LoadError", L"LoadImage Error", MB_ICONERROR);
        return NULL;
    }

    return hBmp;
}

HBITMAP ResourceManager::LoadBmpNpcs(const std::wstring& npcName) {
    if (npcName == L"Лоран") return LoadBMP(L"Loran.bmp");
    else if (npcName == L"Ними") return LoadBMP(L"Nimi.bmp");
    else if (npcName == L"Элиор и Нивра") return LoadBMP(L"Elion.bmp");
    else return LoadBMP(L"hohoi2.bmp"); // default
}

bool ResourceManager::LoadWorldBackgrounds() {
    worldBackgrounds[ANGER] = LoadBMP(L"AngerBack.bmp");
    worldBackgrounds[SADNESS] = LoadBMP(L"SadBack.bmp");
    worldBackgrounds[FEAR] = LoadBMP(L"FearBack.bmp");
    worldBackgrounds[POWER] = LoadBMP(L"PowerBack.bmp");
    worldBackgrounds[CALM] = LoadBMP(L"WhiteBack.bmp"); // Заглушка
    worldBackgrounds[JOY] = LoadBMP(L"WhiteBack.bmp");  // Заглушка

    for (int i = 0; i < COUNT_Emotions; i++) {
        if (!worldBackgrounds[i]) {
            MessageBoxW(NULL, L"Failed to load world background", L"Error", MB_OK);
            return false;
        }
    }
    return true;
}

bool ResourceManager::LoadAllBMP() {

    hBack = LoadBMP(L"SadBack.bmp");         // Фоновый битмап
    backScales = LoadBMP(L"WhiteBack.bmp");     // Битмап для шкал
    backReplace = LoadBMP(L"WhiteBack.bmp");    // Битмап для замен
    backHero = LoadBMP(L"WhiteBack.bmp");      // Битмап для героя
    backCharacter = LoadBMP(L"WhiteBack.bmp"); // Битмап для персонажа
    backMainText = LoadBMP(L"WhiteBack.bmp");   // Битмап для основного текста
    Hero.Icon = LoadBMP(L"GG.bmp");
    ButtonStart = LoadBMP(L"buttonStart.bmp");

    if (!LoadWorldBackgrounds()) return false;

    if (!hBack) return false; // Проверка загрузки 
    if (!backScales) return false;
    if (!backReplace) return false;
    if (!backHero) return false;
    if (!backCharacter) return false;
    if (!backMainText) return false;
    if (!Hero.Icon) return false;
    if (!ButtonStart) return false;

    return true;

}

HBITMAP ResourceManager::GethBack() { return hBack; }
HBITMAP ResourceManager::GetBackScales() { return backScales; }
HBITMAP ResourceManager::GetBackReplace() { return backReplace; }
HBITMAP ResourceManager::GetBackHero() { return backHero; }
HBITMAP ResourceManager::GetBackCharacter() { return backCharacter; }
HBITMAP ResourceManager::GetbackMainText() { return backMainText; }
HBITMAP ResourceManager::GetGG() { return  Hero.Icon; }
HBITMAP ResourceManager::GetButtonStart() { return ButtonStart; }


void ResourceManager::Cleanup() {

    if (hBack) { DeleteObject(hBack); hBack = nullptr; }
    if (backScales) { DeleteObject(backScales); backScales = nullptr; }
    if (backReplace) { DeleteObject(backReplace); backReplace = nullptr; }
    if (backHero) { DeleteObject(backHero); backHero = nullptr; }
    if (backCharacter) { DeleteObject(backCharacter); backCharacter = nullptr; }
    if (backMainText) { DeleteObject(backMainText); backMainText = nullptr; }
    if (Hero.Icon) { DeleteObject(Hero.Icon);  Hero.Icon = nullptr; }
    if (ButtonStart) { DeleteObject(ButtonStart); ButtonStart = nullptr; }

    for (int i = 0; i < Characters.size(); i++) {

        if (Characters[i].icon) { DeleteObject(Characters[i].icon); Characters[i].icon = nullptr; }

    }

}


bool ResourceManager::LoadTextures() {

    name_textures = r_FileManager.FindAllFiles("data", ".bmp");

    if (name_textures.empty()) { 
        MessageBoxW(NULL, L"files empty", L"sosi", MB_ICONERROR);
        return false;
    
    }
}
