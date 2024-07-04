#pragma once

void(*FireSingleOriginal)(AFortWeapon*);
void FireSingle(AFortWeapon* FortWeapon)
{
	if (Settings::Misc::BulletTraces)
	{
		FVector CameraForward = UKismetMathLibrary::GetForwardVector(Variables::CameraRotation);

		if (Variables::GetDamageStartLocation::bSilentAimActive)
		{
			CameraForward = UKismetMathLibrary::GetForwardVector(Variables::GetDamageStartLocation::LastTargetRotation);
		}

		FVector Muzzle = FortWeapon->GetMuzzleLocation();
		FVector MaxLocation = Variables::CameraLocation + (CameraForward * 20000.0); //200m

		FVector BulletImpact = MaxLocation;
		FHitResult OutHit;
		TArray<AActor*> ActorsToIgnore;

		if (UWorld* GWorld = UWorld::GetWorld())
		{
			if (UKismetSystemLibrary::LineTraceSingle(GWorld, Muzzle, MaxLocation, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, false))
			{
				BulletImpact = OutHit.ImpactPoint;
			}

			if (Variables::LastBulletIndexPushed > 4)
			{
				Variables::LastBulletIndexPushed = 0;
			}

			Variables::Last5BulletImpacts[Variables::LastBulletIndexPushed] = BulletImpact;
			Variables::Last5BulletImpactsMuzzle[Variables::LastBulletIndexPushed] = Muzzle;

			Variables::LastBulletIndexPushed += 1;
		}
	}

	static UObject* FortWeaponRangedClass = nullptr;
	if (!FortWeaponRangedClass) FortWeaponRangedClass = UObject::FindObject(L"FortniteGame.FortWeaponRanged");
	bool CurrentWeaponIsARanged = UGameplayStatics::ObjectIsA(FortWeapon, FortWeaponRangedClass);
	if (CurrentWeaponIsARanged)
	{
		auto GetWeaponStats = reinterpret_cast<UObject* (*)(AFortWeapon*)>(GameBase + Offsets::GetWeaponStats);
		auto WeaponStats = reinterpret_cast<UObject* (__cdecl*)(AFortWeapon*, uint64_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, GetWeaponStats);
		if (WeaponStats)
		{
			if (Settings::Exploits::NoSpread)
			{
				TArray<UObject*> PerBulletSpread = *(TArray<UObject*>*)(WeaponStats + 0x328);
				for (int i = 0; i < PerBulletSpread.Count; i++)
				{
					UObject* BulletSpread = PerBulletSpread[i];
					*(float*)(BulletSpread + 0x04) = FLT_MIN; // Spread
				}
			}

			if (Settings::Exploits::NoRecoil)
			{
				TArray<UObject*> PerBulletRecoil = *(TArray<UObject*>*)(WeaponStats + 0x308);
				for (int i = 0; i < PerBulletRecoil.Count; i++)
				{
					UObject* BulletRecoil = PerBulletRecoil[i];
					*(float*)(BulletRecoil + 0x04) = FLT_MIN; // MinVertical
					*(float*)(BulletRecoil + 0x08) = FLT_MIN; // MaxVertical
					*(float*)(BulletRecoil + 0x0c) = FLT_MIN; // MinHorizontal
					*(float*)(BulletRecoil + 0x10) = FLT_MIN; // MaxHorizontal
				}
			}
		}
	}

	reinterpret_cast<void* (__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, FireSingleOriginal);
}