#include "Dialog.h"
#include "Render.h"  

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
    , dialogStepCounter(0)
{
}

void DialogSystem::StartDialogInWorld() {
    if (isActive) {
        OutputDebugStringW(L"[DIALOG] Предупреждение: диалог уже активен!");
        return;
    }

    dialogStepCounter = 0;

    OutputDebugStringW(L"[DIALOG] ===== StartDialogInWorld() ВЫЗВАН =====");

    wchar_t debug[256];

    swprintf(debug, 256, L"[DIALOG] Hero.current_loc = %d", Hero.current_loc);
    OutputDebugStringW(debug);

    Emotion_ currentWorld = Worlds[Hero.current_loc].linked_emotion;
    swprintf(debug, 256, L"[DIALOG] Текущий мир: %s (эмоция: %d)",
        Worlds_Names[currentWorld].c_str(), currentWorld);
    OutputDebugStringW(debug);

    // Находим ВСЕХ NPC в текущем мире
    availableNPCs.clear();
    for (size_t i = 0; i < Characters.size(); i++) {
        const NPC& npc = Characters[i];
        if (npc.world_link == currentWorld) {
            availableNPCs.push_back(&Characters[i]);
        }
    }

    swprintf(debug, 256, L"[DIALOG] Всего NPC в этом мире: %zu", availableNPCs.size());
    OutputDebugStringW(debug);

    if (availableNPCs.empty()) {
        OutputDebugStringW(L"[DIALOG] ОШИБКА: В мире нет NPC!");
        isActive = false;
        return;
    }

  
    int completedDialogs = 0;

    auto it = d_Logic.dialogsCompletedByWorld.find(currentWorld);
    if (it != d_Logic.dialogsCompletedByWorld.end()) {
        completedDialogs = it->second;
    }

    swprintf(debug, 256, L"[DIALOG] Завершено диалогов в мире: %d", completedDialogs);
    OutputDebugStringW(debug);

    // Берем NPC по порядку: 0, 1, 2...
    int npcIndex = completedDialogs;

    if (npcIndex >= availableNPCs.size()) {
        npcIndex = 0;
    }

    currentNPC = availableNPCs[npcIndex];
    currentTextID = 0;
    isChoosingNPC = false;
    isActive = true;

    swprintf(debug, 256, L"[DIALOG] Начинаем диалог с NPC[%d]: %s",
        npcIndex, currentNPC->name.c_str());
    OutputDebugStringW(debug);

    // Находим первую реплику
    currentText = nullptr;
    for (DialogText& text : currentNPC->texts) {
        if (text.id == currentTextID) {
            currentText = &text;
            break;
        }
    }

    if (!currentText) {
        OutputDebugStringW(L"[DIALOG] ОШИБКА: не найден текст с ID 0");
        isActive = false;
    }
}

void DialogSystem::ProcessInput(int keyCode) {
    if (!isActive) return;

    if (isChoosingNPC) {
        switch (keyCode) {
        case VK_UP:
            selectedAnswer = (selectedAnswer - 1 + availableNPCs.size()) % availableNPCs.size();
            break;
        case VK_DOWN:
            selectedAnswer = (selectedAnswer + 1) % availableNPCs.size();
            break;
        case VK_RETURN:
        case VK_SPACE:
            currentNPC = availableNPCs[selectedAnswer];
            currentTextID = 0;
            isChoosingNPC = false;

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

                dialogStepCounter++;

                if (dialogStepCounter > 100) {
                    wchar_t debug[256];
                    swprintf(debug, 256, L"[DIALOG] ПРЕДЕЛ: Диалог слишком длинный (100+ шагов) NPC: %s, Текст ID: %d",
                        currentNPC ? currentNPC->name.c_str() : L"null",
                        currentText ? currentText->id : -1);
                    OutputDebugStringW(debug);

                    if (currentNPC) {
                        d_Logic.OnDialogCompleted(currentNPC->world_link);
                    }
                    isActive = false;
                    currentNPC = nullptr;
                    currentText = nullptr;
                    dialogStepCounter = 0;
                    return;
                }

                DialogAnswer& answer = currentText->answers[selectedAnswer];
                d_Logic.ChangeEmotions(answer.emotion, answer.sign);

                currentTextID = answer.next_text_id;

                if (currentTextID == -1) {
                    wchar_t debugMsg[256];
                    swprintf(debugMsg, 256, L"[DIALOG] Диалог завершен нормально, шагов: %d", dialogStepCounter);
                    OutputDebugStringW(debugMsg);

                    if (currentNPC) {
                        d_Logic.OnDialogCompleted(currentNPC->world_link);
                    }
                    else {
                        d_Logic.OnDialogCompleted();
                    }

                    isActive = false;
                    currentNPC = nullptr;
                    currentText = nullptr;
                    dialogStepCounter = 0;
                    return;
                }

                currentText = nullptr;
                for (DialogText& text : currentNPC->texts) {
                    if (text.id == currentTextID) {
                        currentText = &text;
                        break;
                    }
                }

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
                    dialogStepCounter = 0;
                }

                selectedAnswer = 0;
                game.counterChoices++;
            }
            break;
        case VK_ESCAPE:
            isActive = false;
            currentNPC = nullptr;
            dialogStepCounter = 0;
            break;
        }
    }
}

void DialogSystem::Render(HDC hdc) {
    if (!isActive) return;

    if (isChoosingNPC) {
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

        if (selectedAnswer >= 0 && selectedAnswer < availableNPCs.size()) {
            NPC* previewNPC = availableNPCs[selectedAnswer];
            if (previewNPC && previewNPC->icon) {
                d_Render.ShowBMP(hdc,
                    1520, 40, 370, 560,
                    previewNPC->icon,
                    true);
            }
        }

        d_Render.ShowText(hdc, L"↑↓: Выбор   Enter: Подтвердить   ESC: Выход", 470, 590, 20);
    }
    else if (currentNPC && currentText) {
        if (currentNPC->icon) {
            d_Render.ShowBMP(hdc,
                1520, 40, 370, 560,
                currentNPC->icon,
                 false, true);
        }

        d_Render.ShowText(hdc, currentNPC->name + L":", 470, 40, 28);
        d_Render.ShowText(hdc, currentText->text, 470, 80, 24);

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