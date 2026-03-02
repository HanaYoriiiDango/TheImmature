#pragma once
#include "Global.h"
#include "FileSystem.h"


// src/render/ResourceManager.h
// Пока здесь НИЧЕГО не подключаем и не пишем.

class ResourceManager {
private:
    // === Битмапы ===
    HBITMAP hBack = nullptr;          // Фоновый битмап
    HBITMAP backScales = nullptr;     // Битмап для шкал
    HBITMAP backReplace = nullptr;    // Битмап для замен
    HBITMAP backHero = nullptr;       // Битмап для героя
    HBITMAP backCharacter = nullptr;  // Битмап для персонажа
    HBITMAP backMainText = nullptr;   // Битмап для основного текста
    HBITMAP ButtonStart = nullptr;

    std::vector<std::string> name_textures;


    HBITMAP worldBackgrounds[COUNT_Emotions]; 

    FileSystem& r_FileManager;


public:

    ResourceManager(FileSystem& fm) : r_FileManager(fm) {} 


    ~ResourceManager() {
        Cleanup();  // Автоматическая очистка при уничтожении объекта
    }

    bool LoadWorldBackgrounds(); // Новый метод
    HBITMAP GetWorldBackground(Emotion_ emotion) {
        if (emotion >= 0 && emotion < COUNT_Emotions)
            return worldBackgrounds[emotion];
        return hBack; // fallback
    }

    bool FindFiles(const wchar_t* filename);
    HBITMAP LoadBMP(const wchar_t* name);
    HBITMAP LoadBmpNpcs(const std::wstring& npcName);

    bool LoadAllBMP();
    void Cleanup();

    bool LoadTextures();


    // Getters

    HBITMAP GethBack();
    HBITMAP GetBackScales();
    HBITMAP GetBackReplace();
    HBITMAP GetBackHero();
    HBITMAP GetBackCharacter();
    HBITMAP GetbackMainText();
    HBITMAP GetGG();
    HBITMAP GetButtonStart();
};
