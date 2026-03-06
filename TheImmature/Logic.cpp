#include "Global.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Logic.h"
#include "Render.h"

using namespace std;

// Реализации методов GameLogicSystem
Emotion_ GameLogicSystem::GetOpposite(Emotion_ feels) {
    switch (feels) {
    case SADNESS: return JOY;
    case JOY: return SADNESS;
    case FEAR: return POWER;
    case POWER: return FEAR;
    case ANGER: return CALM;
    case CALM: return ANGER;
    default: COUNT_Emotions; // для обработки ошибок 
    }
}
bool GameLogicSystem::LimitCheck(int value) {

    if (value <= 2 || value >= 98) return true;
    else return false;

}

bool GameLogicSystem::HeroLocCheck() {
    return Worlds[Hero.current_loc].is_locked;

}

Emotion_ GameLogicSystem::DetectedEmotion(int feels) {

    if (LimitCheck(Hero.emotions[feels])) return (Emotion_)feels;
    return COUNT_Emotions;

}

void GameLogicSystem::LockedValue(Emotion_ feels) {

    int value = Hero.emotions[feels];

    if (value < 0) value = 0;
    if (value > 100) value = 100;

    Hero.emotions[feels] = value;

}

void GameLogicSystem::MovingPlayer() {
    vector<int> available_worlds;

    // Собираем все открытые миры
    for (int j = 0; j < Emotion.size(); j++) {
        if (!Worlds[j].is_locked) {
            available_worlds.push_back(j);
        }
    }

    if (!available_worlds.empty()) {
        int random_index = rand() % available_worlds.size();
        Hero.current_loc = available_worlds[random_index];
       /* cout << ">> Переход в " << Worlds_Names[Hero.current_loc] << endl;*/

        vector<int>().swap(available_worlds);

    }
    else {
        Hero.life = false;
        vector<int>().swap(available_worlds);

        cout << ">> Все миры закрыты! Игра завершена.\n";
    }
}

void GameLogicSystem::ChangeGamerule() {

    // ПРОПУСКАЕМ УЖЕ ОБРАБОТАННЫЕ ПАРЫ МИРОВ
    bool processedPairs[COUNT_Emotions] = { false };

    // Сначала проверяем какие миры нужно ЗАКРЫТЬ
    for (int i = 0; i < COUNT_Emotions; i++) {
        if (processedPairs[i]) continue; // уже обработали эту пару

        if (LimitCheck(Hero.emotions[i])) {
            Emotion_ feels = (Emotion_)i;
            Emotion_ OppositeWorld = GetOpposite(feels);

            if (feels >= 0 && feels < COUNT_Emotions &&
                OppositeWorld >= 0 && OppositeWorld < COUNT_Emotions) {

                // Закрываем оба мира и помечаем пару как обработанную
                if (!Worlds[feels].is_locked || !Worlds[OppositeWorld].is_locked) {
                    Worlds[feels].is_locked = true;
                    Worlds[OppositeWorld].is_locked = true;
                    processedPairs[feels] = true;
                    processedPairs[OppositeWorld] = true;
                    //cout << "Закрыты: " << data.Worlds[feels].name << " и " << data.Worlds[OppositeWorld].name << endl;
                }
            }
        }
    }

    // Сбрасываем флаги для открытия
    bool processedPairsOpen[COUNT_Emotions] = { false };

    // Затем проверяем какие миры нужно ОТКРЫТЬ
    for (int i = 0; i < COUNT_Emotions; i++) {
        if (processedPairsOpen[i]) continue; // 

        if (Hero.emotions[i] > 10 && Hero.emotions[i] < 90) {
            Emotion_ OppositeWorld = GetOpposite((Emotion_)i);

            if (i >= 0 && i < COUNT_Emotions &&
                OppositeWorld >= 0 && OppositeWorld < COUNT_Emotions) {

                // Открываем оба мира и помечаем пару как обработанную
                if (Worlds[i].is_locked || Worlds[OppositeWorld].is_locked) {
                    Worlds[i].is_locked = false;
                    Worlds[OppositeWorld].is_locked = false;
                    processedPairsOpen[i] = true;
                    processedPairsOpen[OppositeWorld] = true;
                    //cout << "Открыты: " << Worlds[i].name << " и " << Worlds[OppositeWorld].name << endl;
                }
            }
        }
    }

    // Проверка игрока
    if (Worlds[Hero.current_loc].is_locked) {
        //cout << "Игрок был в закрытом мире: " << Worlds_Names[Hero.current_loc] << endl;
        MovingPlayer();
    }
}


void GameLogicSystem::Transfuse(Emotion_ feels) {

    Emotion_ opposite_emotion = GetOpposite(feels);
    int new_value = 100 - Hero.emotions[feels];
    Hero.emotions[opposite_emotion] = new_value;
    LockedValue(feels);
    LockedValue(opposite_emotion);

}

void GameLogicSystem::Addition(Emotion_ feels, vector<Emotion_> Array) {

    Hero.emotions[feels] += dominationRate;
    Transfuse(feels);

    for (int i = 0; i < Array.size(); i++) {

        ArrayNum = Array[i];

        if (ArrayNum != feels) {

            Hero.emotions[ArrayNum] -= passiveRate;
            Transfuse(ArrayNum);

        }
    }
}

void GameLogicSystem::Subtraction(Emotion_ feels, vector<Emotion_> Array) {

    Hero.emotions[feels] -= dominationRate;
    Transfuse(feels);

    for (int j = 0; j < Array.size(); j++) {

        ArrayNum = Array[j];

        if (ArrayNum != feels) {

            Hero.emotions[ArrayNum] += passiveRate;
            Transfuse(ArrayNum);

        }
    }
}

void GameLogicSystem::ChangeEmotions(Emotion_ DominationEmotion, bool sign) {

    vector<Emotion_>().swap(Positive);
    vector<Emotion_>().swap(Negative);

    for (int i = 0; i < Emotion.size(); i++) {

        if (Emotion[i] % 2 == 0) {

            Positive.push_back({ Emotion[i] });

        }
        else {

            Negative.push_back({ Emotion[i] });

        }
    }
    if (DominationEmotion % 2 == 0) {

        sign ? Addition(DominationEmotion, Positive) : Subtraction(DominationEmotion, Positive);

    }
    else {

        sign ? Addition(DominationEmotion, Negative) : Subtraction(DominationEmotion, Negative);

    }

    vector<Emotion_>().swap(Positive);
    vector<Emotion_>().swap(Negative);

    ChangeGamerule(); // изменяем игровые правила

}

// 1. Метод подсчета NPC в мире
int GameLogicSystem::CountNPCsInWorld(Emotion_ world) {
    int count = 0;
    for (const NPC& npc : Characters) {
        if (npc.world_link == world) {
            count++;
        }
    }
    return count;
}

// Добавим отладочный вывод:
void GameLogicSystem::OnDialogCompleted(Emotion_ world) {
    // Увеличиваем счетчик завершенных диалогов в этом мире
    dialogsCompletedByWorld[world]++;

    int totalNPCs = CountNPCsInWorld(world);
    int completed = dialogsCompletedByWorld[world];

    // Выводим всех NPC в этом мире для отладки
    for (size_t i = 0; i < Characters.size(); i++) {
        const NPC& npc = Characters[i];
        if (npc.world_link == world) {
           
        }
    }

    if (totalNPCs > 0 && completed >= totalNPCs) {
        // ВСЕ NPC в этом мире пройдены
        OutputDebugStringA("[LOGIC] >>> Все NPC пройдены! Запускаем выбор мира.");
        StartWorldSelection();

        // Сбрасываем счетчик для этого мира
        dialogsCompletedByWorld[world] = 0;
    }
    else {
        OutputDebugStringA("[LOGIC] >>> Еще есть NPC. Запускаем следующий диалог.");
        g_NeedAutoStartDialog = true;
    }
}

void GameLogicSystem::OnDialogCompleted() {
    OnDialogCompleted(Worlds[Hero.current_loc].linked_emotion);
}

void GameLogicSystem::StartWorldSelection() {
    isSelectingWorld = true;
    selectedPortal = 0;
    game.Current_State = WORLD_SELECTION;

    OutputDebugStringA("[LOGIC] StartWorldSelection() - выбор миров");
}

// 4. Обработка ввода при выборе мира
void GameLogicSystem::ProcessWorldSelection(int keyCode) {
    if (!isSelectingWorld) return;

    std::vector<int> availablePortals;
    for (int i = 0; i < Worlds[Hero.current_loc].portal.size(); i++) {
        Portal_& portal = Worlds[Hero.current_loc].portal[i];
        if (portal.open && Worlds[portal.target].is_available && !Worlds[portal.target].is_locked) {
            availablePortals.push_back(i);
        }
    }

    if (availablePortals.empty()) return;

    switch (keyCode) {
    case VK_UP:
        selectedPortal = (selectedPortal - 1 + availablePortals.size()) % availablePortals.size();
        break;
    case VK_DOWN:
        selectedPortal = (selectedPortal + 1) % availablePortals.size();
        break;
    case VK_RETURN:
    case VK_SPACE:
        if (selectedPortal >= 0 && selectedPortal < availablePortals.size()) {
            int realPortalIndex = availablePortals[selectedPortal];
            Portal_& portal = Worlds[Hero.current_loc].portal[realPortalIndex];

            if (portal.open && Worlds[portal.target].is_available && !Worlds[portal.target].is_locked) {
                // Запоминаем старый мир для сброса счетчика
                Emotion_ oldWorld = Worlds[Hero.current_loc].linked_emotion;

                // Меняем мир
                Hero.current_loc = portal.target;
                Emotion_ newWorld = Worlds[Hero.current_loc].linked_emotion;

                // Сбрасываем счетчики
                ResetDialogCounterForWorld(oldWorld);
                ResetDialogCounterForWorld(newWorld);

                // Возвращаем состояние
                isSelectingWorld = false;
                game.Current_State = DIALOG;

                // ⭐⭐⭐ УСТАНАВЛИВАЕМ ФЛАГ ДЛЯ АВТОЗАПУСКА ДИАЛОГА ⭐⭐⭐
                g_NeedAutoStartDialog = true;
                OutputDebugStringA("[LOGIC] ===== ФЛАГ УСТАНОВЛЕН! =====");

                return;
            }
        }
        break;
    case VK_ESCAPE:
        isSelectingWorld = false;
        game.Current_State = DIALOG;
        break;
    }
}

// 3. SelectWorld() - упрощаем, т.к. логика перенесена в ProcessWorldSelection
void GameLogicSystem::SelectWorld() {
    // Теперь логика в ProcessWorldSelection
}

void GameLogicSystem::RenderWorldSelection(HDC hdc) {
    if (!isSelectingWorld || !l_Render) return;

    // Собираем доступные порталы
    std::vector<Portal_> availablePortals;
    for (const Portal_& portal : Worlds[Hero.current_loc].portal) {
        if (portal.open && Worlds[portal.target].is_available && !Worlds[portal.target].is_locked) {
            availablePortals.push_back(portal);
        }
    }

    // Заголовок
    l_Render->ShowANSIText(hdc, "Древо Перехода", 800, 100, 36);
    l_Render->ShowANSIText(hdc, Worlds[Hero.current_loc].name + ":", 800, 150, 28);
    l_Render->ShowANSIText(hdc, "Куда отправишься дальше?", 800, 180, 24);

    if (availablePortals.empty()) {
        l_Render->ShowANSIText(hdc, "Нет доступных миров для перехода", 800, 250, 24);
        return;
    }

    // Список ДОСТУПНЫХ миров
    for (int i = 0; i < availablePortals.size(); i++) {
        const Portal_& portal = availablePortals[i];
        std::string worldText = Worlds[portal.target].name;

        // Показываем сколько NPC в целевом мире
        int npcCount = CountNPCsInWorld(portal.target);
        std::string status = " [" + std::to_string(npcCount) + " NPC]";
        worldText += status;

        // Подсветка выбранного
        if (i == selectedPortal) {
            worldText = ">> " + worldText + " <<";
        }
        else {
            worldText = std::to_string(i + 1) + ") " + worldText;
        }

        l_Render->ShowANSIText(hdc, worldText, 800, 250 + i * 50, 24);
    }

    // Подсказка
    l_Render->ShowANSIText(hdc, "Стрелки: Выбор   Enter: Подтвердить   ESC: Выход",
        800, 550, 20);
    
    // Отладочная информация
    char debug[256];
    sprintf_s(debug, sizeof(debug), "Доступно миров: %zd", availablePortals.size());
    l_Render->ShowANSIText(hdc, debug, 800, 600, 18);
}

// 7. Сброс счетчика
void GameLogicSystem::ResetDialogCounterForWorld(Emotion_ world) {
    dialogsCompletedByWorld[world] = 0;
    char debug[256];
    sprintf_s(debug, sizeof(debug), "[LOGIC] Сброс счетчика для мира: %s",
        Worlds_Names[world].c_str());
    OutputDebugStringA(debug);
}

int GameLogicSystem::GetFirstNpcIndexInWorld(Emotion_ world) {
    for (int i = 0; i < Characters.size(); i++) {
        if (Characters[i].world_link == world) {
            return i; // Возвращаем ПЕРВОГО найденного NPC
        }
    }
    return 0; // fallback
}

void GameLogicSystem::StartNextDialogInWorld(Emotion_ world) {
    static std::map<Emotion_, int> currentNPCIndex;

    // Ищем NPC в этом мире
    std::vector<int> npcIndices;
    for (int i = 0; i < Characters.size(); i++) {
        if (Characters[i].world_link == world) {
            npcIndices.push_back(i);
        }
    }

    if (npcIndices.empty()) {
        // Нет NPC - предлагаем выбор мира
        StartWorldSelection();
        return;
    }

    // Берем следующего NPC
    if (currentNPCIndex[world] >= npcIndices.size()) {
        // Все NPC пройдены
        StartWorldSelection();
        currentNPCIndex[world] = 0; // Сброс
        return;
    }

    // Начинаем диалог с текущим NPC
    int npcIndex = npcIndices[currentNPCIndex[world]];
    currentNPCIndex[world]++; // Увеличиваем для следующего раза

    // Нужно как-то вызвать StartDialogWithNPC или аналогичную функцию
    // Если нет такой функции, то нужно изменить архитектуру
}
