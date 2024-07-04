#pragma once

void* (*GetDamageStartLocationOriginal)(AFortWeapon*, FVector*, FRotator*, int);
void* GetDamageStartLocation(AFortWeapon* FortWeapon, FVector* Location, FRotator* Rotation, int a4)
{
    auto ReturnValue = reinterpret_cast<void* (__cdecl*)(AFortWeapon*, FVector*, FRotator*, int, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, Location, Rotation, a4, SpoofCode, GetDamageStartLocationOriginal);

    if (Variables::GetDamageStartLocation::bSilentAimActive && Variables::GetDamageStartLocation::SilentLocationTarget)
    {
        FRotator PerfectRotation = UKismetMathLibrary::FindLookAtRotation(*Location, Variables::GetDamageStartLocation::SilentLocationTarget);

        Variables::GetDamageStartLocation::LastTargetRotation = PerfectRotation;

        *Rotation = PerfectRotation;
    }

    return ReturnValue;
}