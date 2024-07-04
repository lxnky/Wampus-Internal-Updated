#pragma once

void (*GetCameraViewPointOriginal)(APlayerCameraManager*, FVector*, FRotator*);
void GetCameraViewPoint(APlayerCameraManager* PlayerCameraManager, FVector* OutLocation, FRotator* OutRotation)
{
	reinterpret_cast<void(__cdecl*)(APlayerCameraManager*, FVector*, FRotator*, uintptr_t, void*)>(DoSpoofCall)(PlayerCameraManager, OutLocation, OutRotation, SpoofCode, GetCameraViewPointOriginal);

	static bool Old_bShouldStartAtLocationTarget = false;
	static FVector StaticStickyLocation = FVector();

	if (Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget != Old_bShouldStartAtLocationTarget)
	{
		StaticStickyLocation = *OutLocation;

		Old_bShouldStartAtLocationTarget = Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget;
	}

	if (Settings::Aimbot::AimType == 2 && Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget)
	{
		*OutLocation = Variables::GetPlayerViewPoint::OriginalLocation;
		*OutRotation = Variables::GetPlayerViewPoint::OriginalRotation;
	}
	else if (Settings::Aimbot::AimType == 3 && Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget)
	{
		if (Settings::Aimbot::StickySilent::StickyCameraSpin && StaticStickyLocation)
		{
			*OutLocation = StaticStickyLocation;
		}
		else
		{
			*OutLocation = Variables::GetCameraViewPoint::StickyLocation;
		}

		*OutRotation = UKismetMathLibrary::FindLookAtRotation(*OutLocation, Variables::GetPlayerViewPoint::SilentLocationTarget);

		Variables::GetCameraViewPoint::LastStickyRotation = *OutRotation;
	}
}