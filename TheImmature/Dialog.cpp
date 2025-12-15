// Dialog.cpp
#include "Dialog.h"

DialogSystem::DialogSystem(JsonManager& jsonManager, GameLogicSystem& logic, RenderSystem& render)
    : d_JsonManager(jsonManager)
    , d_Logic(logic)
    , d_Render(render)
    , currentNPC(nullptr)
    , currentText(nullptr)
    , selectedAnswer(0)
    , isActive(false)
    , currentTextID(0)
    , isChoosingNPC(false)
{
}

void DialogSystem::StartDialogInWorld() {
    // ⭐⭐⭐ ДОБАВЬ ЭТОТ ОТЛАДОЧНЫЙ ВЫВОД ⭐⭐⭐
    OutputDebugStringW(L"[DIALOG] ===== StartDialogInWorld() ВЫЗВАН =====");

    wchar_t debug[256];

    // 1. Какая текущая локация?
    swprintf(debug, 256, L"[DIALOG] Hero.current_loc = %d", Hero.current_loc);
    OutputDebugStringW(debug);

    // 2. Какой мир (эмоция) связан с локацией?
    Emotion_ currentWorld = Worlds[Hero.current_loc].linked_emotion;
    swprintf(debug, 256, L"[DIALOG] Текущий мир: %s (эмоция: %d)",
        Worlds_Names[currentWorld].c_str(), currentWorld);
    OutputDebugStringW(debug);

    // 3. Сколько всего NPC в игре?
    swprintf(debug, 256, L"[DIALOG] Всего Characters в игре: %zu", Characters.size());
    OutputDebugStringW(debug);

    // 4. Какие NPC есть в текущем мире?
    availableNPCs.clear();
    int npcCountInWorld = 0;

    for (size_t i = 0; i < Characters.size(); i++) {
        const NPC& npc = Characters[i];
        if (npc.world_link == currentWorld) {
            availableNPCs.push_back(&Characters[i]);
            npcCountInWorld++;

            swprintf(debug, 256, L"[DIALOG] NPC в мире: %s (world_link: %d)",
                npc.name.c_str(), npc.world_link);
            OutputDebugStringW(debug);

            // 5. Проверим есть ли у NPC тексты?
            swprintf(debug, 256, L"[DIALOG]   У NPC %s текстов: %zu",
                npc.name.c_str(), npc.texts.size());
            OutputDebugStringW(debug);
        }
    }

    swprintf(debug, 256, L"[DIALOG] Всего NPC в этом мире: %d", npcCountInWorld);
    OutputDebugStringW(debug);

    if (availableNPCs.empty()) {
        OutputDebugStringW(L"[DIALOG] ОШИБКА: В мире нет NPC!");
        isActive = false;
        return;
    }

    // 6. Если NPC один - сразу начинаем диалог
    if (availableNPCs.size() == 1) {
        currentNPC = availableNPCs[0];
        currentTextID = 0;
        isChoosingNPC = false;
        isActive = true;

        swprintf(debug, 256, L"[DIALOG] Начинаем диалог с NPC: %s", currentNPC->name.c_str());
        OutputDebugStringW(debug);

        // Находим первую реплику
        currentText = nullptr;
        for (DialogText& text : currentNPC->texts) {
            if (text.id == currentTextID) {
                currentText = &text;
                break;
            }
        }

        if (currentText) {
            swprintf(debug, 256, L"[DIALOG] Найден текст с ID %d: %.20s...",
                currentTextID, currentText->text.substr(0, 20).c_str());
            OutputDebugStringW(debug);
        }
        else {
            OutputDebugStringW(L"[DIALOG] ОШИБКА: не найден текст с ID 0");
            isActive = false;
        }
    }
    else {
        // Если несколько - показываем выбор NPC
        isChoosingNPC = true;
        selectedAnswer = 0;
        isActive = true;
        OutputDebugStringW(L"[DIALOG] Показываем выбор из нескольких NPC");
    }

    OutputDebugStringW(L"[DIALOG] ===== StartDialogInWorld() ЗАВЕРШЕН =====");
}

void DialogSystem::ProcessInput(int keyCode) {
    if (!isActive) return;

    if (isChoosingNPC) {
        // Выбор NPC из нескольких
        switch (keyCode) {
        case VK_UP:
            selectedAnswer = (selectedAnswer - 1 + availableNPCs.size()) % availableNPCs.size();
            break;
        case VK_DOWN:
            selectedAnswer = (selectedAnswer + 1) % availableNPCs.size();
            break;
        case VK_RETURN:
        case VK_SPACE:
            // Выбираем NPC
            currentNPC = availableNPCs[selectedAnswer];
            currentTextID = 0;
            isChoosingNPC = false;

            // Находим первую реплику
            currentText = nullptr;
            for (DialogText& text : currentNPC->texts) {
                if (text.id == currentTextID) {
                    currentText = &text;
                    break;
                }
            }
            break;
        case VK_ESCAPE:
            isActive = false;
            break;
        }
    }
    else {
        // Обычный диалог
        if (!currentText) return;

        switch (keyCode) {
        case VK_UP:
            if (currentText->answers.size() > 0) {
                selectedAnswer = (selectedAnswer - 1 + currentText->answers.size()) % currentText->answers.size();
            }
            break;
        case VK_DOWN:
            if (currentText->answers.size() > 0) {
                selectedAnswer = (selectedAnswer + 1) % currentText->answers.size();
            }
            break;
        case VK_RETURN:
        case VK_SPACE:
            if (currentText->answers.size() > 0 &&
                selectedAnswer >= 0 &&
                selectedAnswer < currentText->answers.size()) {

                // Применяем эффект от ответа
                DialogAnswer& answer = currentText->answers[selectedAnswer];
                d_Logic.ChangeEmotions(answer.emotion, answer.sign);

            
                currentTextID = answer.next_text_id;

                if (currentTextID == -1) { // Диалог завершен
                    OutputDebugStringW(L"[DIALOG] Диалог завершен, уведомляем GameLogicSystem");

                    if (currentNPC) {
                        // Передаем мир NPC
                        d_Logic.OnDialogCompleted(currentNPC->world_link);
                    }
                    else {
                        // Или текущий мир
                        d_Logic.OnDialogCompleted();
                    }

                    isActive = false;
                    currentNPC = nullptr;
                    currentText = nullptr;
                    return; // Выходим сразу, не ищем следующий текст
                }
                

                // Ищем следующий текст (только если диалог не завершен)
                currentText = nullptr;
                for (DialogText& text : currentNPC->texts) {
                    if (text.id == currentTextID) {
                        currentText = &text;
                        break;
                    }
                }

                // Если текста нет - завершаем диалог
                if (!currentText) {
                    OutputDebugStringW(L"[DIALOG] Не найден следующий текст, завершаем диалог");

                    if (currentNPC) {
                        d_Logic.OnDialogCompleted(currentNPC->world_link);
                    }
                    else {
                        d_Logic.OnDialogCompleted();
                    }

                    isActive = false;
                    currentNPC = nullptr;
                }

                // Сбрасываем выбор
                selectedAnswer = 0;
                game.counterChoices++;
            }
            break;
        case VK_ESCAPE:
            isActive = false;
            currentNPC = nullptr;
            break;
        }
    }
}

void DialogSystem::Render(HDC hdc) {
    if (!isActive) return;

    if (isChoosingNPC) {
        // Рендерим выбор NPC
        d_Render.ShowText(hdc, L"Вы можете поговорить с:", 470, 40, 28);

        for (int i = 0; i < availableNPCs.size(); i++) {
            std::wstring npcText = availableNPCs[i]->name;
            if (i == selectedAnswer) {
                npcText = L">> " + npcText + L" <<";
            }
            else {
                npcText = std::to_wstring(i + 1) + L") " + npcText;
            }
            d_Render.ShowText(hdc, npcText, 470, 100 + i * 40, 24);
        }

        // Показываем иконку выбранного NPC в режиме выбора
        if (selectedAnswer >= 0 && selectedAnswer < availableNPCs.size()) {
            NPC* previewNPC = availableNPCs[selectedAnswer];
            if (previewNPC && previewNPC->icon) {
                d_Render.ShowBMP(hdc,
                    1520,    // X координата (правая панель)
                    40,      // Y координата
                    370,     // Ширина
                    560,     // Высота
                    previewNPC->icon,
                    true);   // transparent = true
            }
        }

        d_Render.ShowText(hdc, L"↑↓: Выбор   Enter: Подтвердить   ESC: Выход", 470, 590, 20);
    }
    else if (currentNPC && currentText) {
        // Рендерим обычный диалог

        // 1. Иконка NPC (справа)
        if (currentNPC->icon) {
            d_Render.ShowBMP(hdc,
                1520,    // X координата (правая панель)
                40,      // Y координата
                370,     // Ширина
                560,     // Высота
                currentNPC->icon,
                true);   // transparent = true (убираем черный фон)
        }

        // 2. Имя NPC
        d_Render.ShowText(hdc, currentNPC->name + L":", 470, 40, 28);

        // 3. Текст NPC
        d_Render.ShowText(hdc, currentText->text, 470, 80, 24);

        // 4. Варианты ответов
        for (int i = 0; i < currentText->answers.size(); i++) {
            std::wstring answerText = currentText->answers[i].text;
            if (i == selectedAnswer) {
                answerText = L">> " + answerText + L" <<";
            }
            else {
                answerText = std::to_wstring(i + 1) + L") " + answerText;
            }
            d_Render.ShowText(hdc, answerText, 470, 670 + i * 40, 22);
        }

        d_Render.ShowText(hdc, L"↑↓: Выбор   Enter: Подтвердить   ESC: Выход", 470, 590, 20);
    }
}