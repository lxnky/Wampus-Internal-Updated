#pragma once

float (*GetReloadTimeOriginal)(AFortWeapon*);
float GetReloadTime(AFortWeapon* FortWeapon)
{
	if (Settings::Exploits::InstantReload)
	{
		return FLT_MAX;
	}

	return reinterpret_cast<float(__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, GetReloadTimeOriginal);
}