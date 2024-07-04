#pragma once

float (*GetProjectileSpeedOriginal)(AFortWeapon*);
float GetProjectileSpeed(AFortWeapon* FortWeapon)
{
	auto ReturnValue = reinterpret_cast<float(__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, GetProjectileSpeed);

	if (Settings::Exploits::FastProjectileShoot)
	{
		return ReturnValue + 2.5f;
	}

	return ReturnValue;
}