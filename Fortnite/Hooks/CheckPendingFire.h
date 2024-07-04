#pragma once

void(*CheckPendingFireOriginal)(AFortWeapon*);
void CheckPendingFire(AFortWeapon* FortWeapon)
{
	MessageBoxA(0, "CheckPendingFire", "Hooks", MB_OK);
	reinterpret_cast<void* (__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, CheckPendingFireOriginal);
}