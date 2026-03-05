#include "ResManager.h" 
#include "filesystem"
#include <commdlg.h>

bool ResourceManager::FindFiles(LPCSTR filename) {

    return GetFileAttributesA(filename) != INVALID_FILE_ATTRIBUTES;
}

HBITMAP ResourceManager::LoadBMP(LPCSTR filename) {
    if (!FindFiles(filename)) {
        MessageBoxA(NULL, "No find files", "sosi", MB_ICONERROR);
        return NULL;
    }

    HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmp) {
        MessageBoxA(NULL, "LoadError", "LoadImage Error", MB_ICONERROR);
        return NULL;
    }

    return hBmp;
}

HBITMAP ResourceManager::LoadBmpNpcs(const std::string& npcName) {
    if (npcName == "Лоран") return LoadBMP("Loran.bmp");
    else if (npcName == "Ними") return LoadBMP("Nimi.bmp");
    else if (npcName == "Элиор и Нивра") return LoadBMP("Elion.bmp");
    else return LoadBMP("DefaultBMP.bmp"); // default
}

bool ResourceManager::LoadWorldBackgrounds() {
    worldBackgrounds[ANGER] = LoadBMP("AngerBack.bmp");
    worldBackgrounds[SADNESS] = LoadBMP("SadBack.bmp");
    worldBackgrounds[FEAR] = LoadBMP("FearBack.bmp");
    worldBackgrounds[POWER] = LoadBMP("PowerBack.bmp");
    worldBackgrounds[CALM] = LoadBMP("WhiteBack.bmp"); // Заглушка
    worldBackgrounds[JOY] = LoadBMP("WhiteBack.bmp");  // Заглушка

    for (int i = 0; i < COUNT_Emotions; i++) {
        if (!worldBackgrounds[i]) {
            MessageBoxA(NULL, "Failed to load world background", "Error", MB_OK);
            return false;
        }
    }
    return true;
}

bool ResourceManager::LoadAllBMP() {

    hBack = LoadBMP("SadBack.bmp");         // Фоновый битмап
    backScales = LoadBMP("WhiteBack.bmp");     // Битмап для шкал
    backReplace = LoadBMP("WhiteBack.bmp");    // Битмап для замен
    backHero = LoadBMP("WhiteBack.bmp");      // Битмап для героя
    backCharacter = LoadBMP("WhiteBack.bmp"); // Битмап для персонажа
    backMainText = LoadBMP("WhiteBack.bmp");   // Битмап для основного текста
    Hero.Icon = LoadBMP("GG.bmp");
    ButtonStart = LoadBMP("buttonStart.bmp");

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

//bool ResourceManager::LoadTextures() {
//
//    auto files = r_FileManager.FindAllFiles("data/textures", ".bmp");
//
//    if (files.empty()) {
//        MessageBoxA(NULL, "files empty", "sosi", MB_ICONERROR);
//        return false;
//    
//    }
//
//    int loadedCount = 0;
//
//    for (const auto& filename : files) {
//
//        Texture::ID id = ParseTextureName(filename);
//
//        if (id != Texture::ID::AllDefault || id != Texture::ID::NpcDefault) {
//
//            std::string fullpath = "textures/" + filename;
//            HBITMAP hBitmap = LoadBMP((fullPath).c_str());
//
//
//        }
//
//    }
//
//}
