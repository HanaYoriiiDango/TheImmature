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

HBITMAP ResourceManager::LoadBmpNpcs(const std::string& npcName) { // загружает BMP npc

    if (npcName == "Nimi") return LoadBMP("Nimi.bmp");
    if (npcName == "Elion") return LoadBMP("Elion.bmp");
    if (npcName == "Loran") return LoadBMP("Loran.bmp");
    else return LoadBMP("DefaultBMP.bmp");
}

bool ResourceManager::LoadWorldBackgrounds() {

    Interface.worldBackgrounds[ANGER] = LoadBMP("AngerBack.bmp");
    Interface.worldBackgrounds[SADNESS] = LoadBMP("SadBack.bmp");
    Interface.worldBackgrounds[FEAR] = LoadBMP("FearBack.bmp");
    Interface.worldBackgrounds[POWER] = LoadBMP("PowerBack.bmp");
    Interface.worldBackgrounds[CALM] = LoadBMP("WhiteBack.bmp"); // Заглушка
    Interface.worldBackgrounds[JOY] = LoadBMP("WhiteBack.bmp");  // Заглушка

    for (int i = 0; i < COUNT_Emotions; i++) {
        if (!Interface.worldBackgrounds[i]) {
            MessageBoxA(NULL, "Failed to load world background", "Error", MB_OK);
            return false;
        }
    }
    return true;
}

bool ResourceManager::LoadAllBMP() {

    Interface.hBack = LoadBMP("SadBack.bmp");         // Фоновый битмап
    Interface.backScales = LoadBMP("WhiteBack.bmp");     // Битмап для шкал
    Interface.backReplace = LoadBMP("WhiteBack.bmp");    // Битмап для замен
    Interface.backHero = LoadBMP("WhiteBack.bmp");      // Битмап для героя
    Interface.backCharacter = LoadBMP("WhiteBack.bmp"); // Битмап для персонажа
    Interface.backMainText = LoadBMP("WhiteBack.bmp");   // Битмап для основного текста
    Hero.Icon = LoadBMP("GG.bmp");

    if (!LoadWorldBackgrounds()) return false;

    if (!Interface.hBack) return false; // Проверка загрузки 
    if (!Interface.backScales) return false;
    if (!Interface.backReplace) return false;
    if (!Interface.backHero) return false;
    if (!Interface.backCharacter) return false;
    if (!Interface.backMainText) return false;
    if (!Hero.Icon) return false;

    return true;

}

void ResourceManager::Cleanup() {

    if (Interface.hBack) { DeleteObject(Interface.hBack); Interface.hBack = nullptr; }
    if (Interface.backScales) { DeleteObject(Interface.backScales); Interface.backScales = nullptr; }
    if (Interface.backReplace) { DeleteObject(Interface.backReplace); Interface.backReplace = nullptr; }
    if (Interface.backHero) { DeleteObject(Interface.backHero); Interface.backHero = nullptr; }
    if (Interface.backCharacter) { DeleteObject(Interface.backCharacter); Interface.backCharacter = nullptr; }
    if (Interface.backMainText) { DeleteObject(Interface.backMainText); Interface.backMainText = nullptr; }
    if (Hero.Icon) { DeleteObject(Hero.Icon);  Hero.Icon = nullptr; }

    for (int i = 0; i < Characters.size(); i++) {

        if (Characters[i].icon) { DeleteObject(Characters[i].icon); Characters[i].icon = nullptr; }

    }

}
 