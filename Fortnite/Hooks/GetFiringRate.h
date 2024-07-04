#pragma once

float (*GetFiringRateOriginal)(AFortWeapon*);
float GetFiringRate(AFortWeapon* FortWeapon)
{
	auto ReturnValue = reinterpret_cast<float(__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, GetFiringRateOriginal);

	if (Settings::Exploits::UnlockFireRate)
	{
		return ReturnValue + 1.5f;
	}

	return ReturnValue;
}