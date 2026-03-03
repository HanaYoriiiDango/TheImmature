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
    else return LoadBMP(L"DefaultBMP.bmp"); // default
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


Texture::ID ResourceManager::ParseTextureName(const std::string& filename) {

        // 1. Убираем расширение
        std::string name = filename.substr(0, filename.find_last_of("."));

        // 2. Ищем первый "_"
        size_t pos = name.find('_');
        if (pos == std::string::npos) return Texture::ID::AllDefault;

        std::string prefix = name.substr(0, pos);
        std::string value = name.substr(pos + 1);

        // 3. Фоны
        if (prefix == "bg") {
            if (value == "joy") return Texture::ID::Background_JOY;
            if (value == "sad") return Texture::ID::Background_SAD;
            if (value == "power") return Texture::ID::Background_POWER;
            if (value == "fear") return Texture::ID::Background_FEAR;
            if (value == "calm") return Texture::ID::Background_CALM;
            if (value == "anger") return Texture::ID::Background_ANGER;
        }

        // 4. UI
        if (prefix == "ui") {
            if (value == "back") return Texture::ID::hBack;
            if (value == "scales") return Texture::ID::backScales;
            if (value == "replace") return Texture::ID::backReplace;
            if (value == "hero") return Texture::ID::backHero;
            if (value == "character") return Texture::ID::backCharacter;
            if (value == "maintext") return Texture::ID::backMainText;
            if (value == "button") return Texture::ID::ButtonStart;
        }

        // 5. NPC
        if (prefix == "npc") {
            if (value == "gg") return Texture::ID::HeroGG;
            if (value == "laurent") return Texture::ID::NpcLoran;
            if (value == "nimi") return Texture::ID::NpcNimi;
            if (value == "elion") return Texture::ID::NpcElion;
            if (value == "default") return Texture::ID::NpcDefault; 
        }

        return Texture::ID::AllDefault;

}

bool ResourceManager::LoadTextures() {

    auto files = r_FileManager.FindAllFiles("data/textures", ".bmp");

    if (files.empty()) {
        MessageBoxW(NULL, L"files empty", L"sosi", MB_ICONERROR);
        return false;
    
    }

    for (const auto& filename : files) {

        Texture::ID id = ParseTextureName(filename);

        if (id != Texture::ID::AllDefault || id != Texture::ID::NpcDefault) {

            std::string fullpath = "textures/" + filename;
            HBITMAP hBitmap = LoadBMP();


        }

    }

}
