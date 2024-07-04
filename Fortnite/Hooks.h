#pragma once

void Rehook(APlayerController* PlayerController)
{
	static APlayerController* OldPlayerController = nullptr;

	if (PlayerController != OldPlayerController)
	{
		static UObject* PlayerControllerClass = nullptr;
		if (!PlayerControllerClass) PlayerControllerClass = UObject::FindObject(L"Engine.PlayerController");
		bool PlayerCameraManagerIsA = UGameplayStatics::ObjectIsA(PlayerController, PlayerControllerClass);
		if (PlayerCameraManagerIsA)
		{
			static Hook::NewHook PlayerControllerHook;
			if (PlayerControllerHook.Initialize(PlayerController))
			{
				PlayerControllerHook.Insert(&GetPlayerViewPoint, Offsets::GetPlayerViewPoint, &GetPlayerViewPointOriginal);

				PlayerControllerHook.SwapContext();
			}

			OldPlayerController = PlayerController;
		}
	}

	if (APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager())
	{
		static APlayerCameraManager* OldPlayerCameraManager = nullptr;

		if (PlayerCameraManager != OldPlayerCameraManager)
		{
			static UObject* PlayerCameraManagerClass = nullptr;
			if (!PlayerCameraManagerClass) PlayerCameraManagerClass = UObject::FindObject(L"Engine.PlayerCameraManager");
			bool PlayerCameraManagerIsA = UGameplayStatics::ObjectIsA(PlayerCameraManager, PlayerCameraManagerClass);
			if (PlayerCameraManagerIsA)
			{
				static Hook::NewHook PlayerCameraManagerHook;
				if (PlayerCameraManagerHook.Initialize(PlayerCameraManager))
				{
					PlayerCameraManagerHook.Insert(&GetCameraViewPoint, Offsets::GetCameraViewPoint, &GetCameraViewPointOriginal);

					PlayerCameraManagerHook.SwapContext();
				}

				OldPlayerCameraManager = PlayerCameraManager;
			}
		}
	}

	if (APlayerPawn_Athena_C* AcknowledgedPawn = PlayerController->AcknowledgedPawn())
	{
		uintptr_t SecretAcknowledgedPawn = uintptr_t(AcknowledgedPawn) + Offsets::PlayerVtable;
		static uintptr_t OldSecretAcknowledgedPawn = 0;

		if (SecretAcknowledgedPawn != OldSecretAcknowledgedPawn)
		{
			static Hook::NewHook SecretAcknowledgedPawnHook;
			if (SecretAcknowledgedPawnHook.Initialize((void*)SecretAcknowledgedPawn))
			{
				//SecretAcknowledgedPawnHook.Insert(&GetTargetingTransform, Offsets::GetTargetingTransform, &GetTargetingTransformOriginal);
				SecretAcknowledgedPawnHook.Insert(&CalculateShot, Offsets::GetTargetingTransform, &CalculateShotOriginal);

				SecretAcknowledgedPawnHook.SwapContext();
			}

			OldSecretAcknowledgedPawn = SecretAcknowledgedPawn;
		}

		if (AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon())
		{
			static AFortWeapon* OldCurrentWeapon = nullptr;

			if (CurrentWeapon != OldCurrentWeapon)
			{
				static UObject* FortWeaponRangedClass = nullptr;
				if (!FortWeaponRangedClass) FortWeaponRangedClass = UObject::FindObject(L"FortniteGame.FortWeaponRanged");
				bool CurrentWeaponIsARanged = UGameplayStatics::ObjectIsA(CurrentWeapon, FortWeaponRangedClass);
				if (CurrentWeaponIsARanged)
				{
					static Hook::NewHook CurrentWeaponHook;
					if (CurrentWeaponHook.Initialize(CurrentWeapon))
					{
						CurrentWeaponHook.Insert(&GetReloadTime, Offsets::GetReloadTime, &GetReloadTimeOriginal);
						CurrentWeaponHook.Insert(&GetFiringRate, Offsets::GetFiringRate, &GetFiringRateOriginal);
						CurrentWeaponHook.Insert(&GetProjectileSpeed, Offsets::GetProjectileSpeed, &GetProjectileSpeedOriginal);
						CurrentWeaponHook.Insert(&GetDamageStartLocation, Offsets::GetDamageStartLocation, &GetDamageStartLocationOriginal);
						CurrentWeaponHook.Insert(&FireSingle, Offsets::FireSingle, &FireSingleOriginal);
						CurrentWeaponHook.Insert(&TryToFire, Offsets::TryToFire, &TryToFireOriginal);

						CurrentWeaponHook.SwapContext();
					}

					OldCurrentWeapon = CurrentWeapon;
				}
			}
		}
	}
}