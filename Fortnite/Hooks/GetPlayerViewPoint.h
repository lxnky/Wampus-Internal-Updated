#pragma once

void (*GetPlayerViewPointOriginal)(APlayerController*, FVector*, FRotator*);
void GetPlayerViewPoint(APlayerController* PlayerController, FVector* Location, FRotator* Rotation)
{
	reinterpret_cast<void(__cdecl*)(APlayerController*, FVector*, FRotator*, uintptr_t, void*)>(DoSpoofCall)(PlayerController, Location, Rotation, SpoofCode, GetPlayerViewPointOriginal);

	Variables::GetPlayerViewPoint::OriginalLocation = *Location;
	Variables::GetPlayerViewPoint::OriginalRotation = *Rotation;

	if (Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget && Variables::GetPlayerViewPoint::SilentLocationTarget)
	{
		FRotator PerfectRotation = UKismetMathLibrary::FindLookAtRotation(*Location, Variables::GetPlayerViewPoint::SilentLocationTarget);

		Variables::GetPlayerViewPoint::LastTargetRotation = PerfectRotation;

		*Rotation = PerfectRotation;
	}
}