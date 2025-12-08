#include "systems.h"
#include <iostream>

using namespace std;

bool ResourceManager::FindFiles(const wchar_t* filename) {

    return GetFileAttributesW(filename) != INVALID_FILE_ATTRIBUTES;
}

HBITMAP ResourceManager::LoadBMP(const wchar_t* name) {

    if (!FindFiles(name)) {

        MessageBoxW(NULL, L"No find files", L"sosi", MB_ICONERROR);
        return NULL;
    }

    HBITMAP hBmp = (HBITMAP)LoadImageW(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmp) MessageBox(NULL, L"No find bmp", L"sosi", MB_ICONERROR);
    return hBmp;

}

bool ResourceManager::LoadAllBMP() {

    hBack = LoadBMP(L"SadBack.bmp");         // Фоновый битмап
    backScales = LoadBMP(L"WhiteBack.bmp");     // Битмап для шкал
    backReplace = LoadBMP(L"WhiteBack.bmp");    // Битмап для замен
    backHero = LoadBMP(L"WhiteBack.bmp");      // Битмап для героя
    backCharacter = LoadBMP(L"WhiteBack.bmp"); // Битмап для персонажа
    backMainText = LoadBMP(L"WhiteBack.bmp");   // Битмап для основного текста

    if (!hBack) return false; // Проверка загрузки 
    if (!backScales) return false;
    if (!backReplace) return false;
    if (!backHero) return false;
    if (!backCharacter) return false;
    if (!backMainText) return false;

    return true;

}

HBITMAP ResourceManager::GethBack() { return hBack; }
HBITMAP ResourceManager::GetBackScales() { return backScales; }
HBITMAP ResourceManager::GetBackReplace() { return backReplace; }
HBITMAP ResourceManager::GetBackHero() { return backHero; }
HBITMAP ResourceManager::GetBackCharacter() { return backCharacter; }
HBITMAP ResourceManager::GetbackMainText() { return backMainText; }

void ResourceManager::Cleanup() {

    if (hBack) { DeleteObject(hBack); hBack = nullptr; }
    if (backScales) { DeleteObject(backScales); backScales = nullptr; }
    if (backReplace) { DeleteObject(backReplace); backReplace = nullptr; }
    if (backHero) { DeleteObject(backHero); backHero = nullptr; }
    if (backCharacter) { DeleteObject(backCharacter); backCharacter = nullptr; }
    if (backMainText) { DeleteObject(backMainText); backMainText = nullptr; }

}