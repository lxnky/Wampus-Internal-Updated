#pragma once

void (*TryToFireOriginal)(AFortWeapon*);
void TryToFire(AFortWeapon* FortWeapon)
{
	if (Settings::Exploits::InstaDMAClip)
	{
		if (FortWeapon)
		{
			FString WeaponName = UKismetSystemLibrary::GetObjectName(FortWeapon);
			if (UKismetStringLibrary::Contains(WeaponName, FString(L"DMR"), true, false))
			{
				for (int i = 0; i < FortWeapon->AmmoCount(); i++)
				{
					reinterpret_cast<void(__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, (void*)(GameBase + Offsets::WeaponFireSingle));
				}
			}
		}
	}

	reinterpret_cast<void(__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, TryToFireOriginal);
}