#include "cheat.h"
#include "imgui/imgui.h"

uintptr_t moduleBase = 0;
bool triggerEnabled = false;
bool espEnabled = false;

void SimulateFire()
{
    INPUT inputDown = { 0 };
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &inputDown, sizeof(INPUT));

    Sleep(10);

    INPUT inputUp = { 0 };
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &inputUp, sizeof(INPUT));
}

bool IsValidPointer(uintptr_t pointer)
{
    return pointer && (pointer > 0x10000) && (pointer < 0x7FFFFFFFFFFF);
}

uintptr_t GetEntityFromIndex(int index)
{
    if (!moduleBase || index <= 0) return 0;

    uintptr_t entityList = *(uintptr_t*)(moduleBase + dwEntityList);
    if (!IsValidPointer(entityList)) return 0;

    uintptr_t entity = *(uintptr_t*)(entityList + (0x8 * (index >> 9) + 0x10));
    if (!IsValidPointer(entity)) return 0;

    return *(uintptr_t*)(entity + 0x78 * (index & 0x1FF));
}

void DrawESP()
{
    for (int i = 0; i < 2048; ++i)
    {
        uintptr_t entity = GetEntityFromIndex(i);
        if (!IsValidPointer(entity)) continue;

        int entityHealth = *(int*)(entity + m_iHealth);
        if (entityHealth <= 0) continue;

        uint8_t entityTeam = *(uint8_t*)(entity + m_iTeamNum);

        // Aquí dibujaríamos la caja alrededor de la entidad usando ImGui
        ImGui::Text("Entity ID: %d", i);
        ImGui::Text("Health: %d", entityHealth);
        ImGui::Text("Team: %d", entityTeam);
    }
}

void TriggerThread()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "TriggerBot CS2 - Versión Corregida\n";

    while (true)
    {
        if (!moduleBase)
        {
            moduleBase = (uintptr_t)GetModuleHandleW(L"client.dll");
            if (!moduleBase)
            {
                Sleep(1000);
                continue;
            }
            std::cout << "Base address: 0x" << std::hex << moduleBase << std::dec << "\n";
        }

        if (!triggerEnabled)
        {
            Sleep(10);
            continue;
        }

        uintptr_t localPlayerPawn = *(uintptr_t*)(moduleBase + dwLocalPlayerPawn);
        if (!IsValidPointer(localPlayerPawn))
        {
            Sleep(10);
            continue;
        }

        int crosshairId = *(int*)(localPlayerPawn + m_iIDEntIndex);
        if (crosshairId <= 0 || crosshairId > 2048)
        {
            Sleep(10);
            continue;
        }

        uintptr_t entity = GetEntityFromIndex(crosshairId);
        if (!IsValidPointer(entity))
        {
            std::cout << "No se pudo obtener la entidad para ID: " << crosshairId << "\n";
            Sleep(10);
            continue;
        }

        uint8_t localTeam = *(uint8_t*)(localPlayerPawn + m_iTeamNum);
        uint8_t entityTeam = *(uint8_t*)(entity + m_iTeamNum);
        int entityHealth = *(int*)(entity + m_iHealth);

        std::cout << "LocalTeam: " << (int)localTeam << " | EntityTeam: " << (int)entityTeam
            << " | Health: " << entityHealth << "\n";

        if (entityHealth > 0 && entityTeam != localTeam)
        {
            std::cout << "DISPARANDO!\n";
            SimulateFire();
            Sleep(50);
        }

        Sleep(5);
    }
}