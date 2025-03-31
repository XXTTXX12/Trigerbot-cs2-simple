#pragma once
#include <Windows.h>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h> // Usar DirectXMath en lugar de d3dx11.h

// Offsets actualizados (verifica que sean correctos para tu versión)
constexpr uintptr_t dwLocalPlayerPawn = 0x188CF70;
constexpr uintptr_t m_iIDEntIndex = 0x1458;
constexpr uintptr_t dwEntityList = 0x1A38800;
constexpr uintptr_t m_iTeamNum = 0x3E3;
constexpr uintptr_t m_iHealth = 0x344;

extern uintptr_t moduleBase;
extern bool triggerEnabled;
extern bool espEnabled;

void SimulateFire();
bool IsValidPointer(uintptr_t pointer);
uintptr_t GetEntityFromIndex(int index);
void TriggerThread();
void DrawESP();