#pragma once
#include "Systems.h"

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

public:
    ResourceManager() = default;
    ~ResourceManager() {
        Cleanup();  // Автоматическая очистка при уничтожении объекта
    }

    bool FindFiles(const wchar_t* filename);
    HBITMAP LoadBMP(const wchar_t* name);
    bool LoadAllBMP();
    void Cleanup();

    // Getters

    HBITMAP GethBack();
    HBITMAP GetBackScales();
    HBITMAP GetBackReplace();
    HBITMAP GetBackHero();
    HBITMAP GetBackCharacter();
    HBITMAP GetbackMainText();

};
