#pragma once

class UObject
{
public:
	UObject* GetOuterObject()
	{
		return *(UObject**)(this + 0x20);
	}

	void ProcessEvent(UObject* UFunction, void* Params)
	{
		if (auto VTable = *(void***)this)
		{
			reinterpret_cast<void(__cdecl*)(UObject*, UObject*, void*, uintptr_t, void*)>(DoSpoofCall)(this, UFunction, Params, SpoofCode, VTable[Offsets::ProcessEvent]);
		}
	}

	static UObject* StaticFindObject(UObject* Class, UObject* Outer, const wchar_t* Name, bool ExactClass)
	{
		return reinterpret_cast<UObject * (__cdecl*)(UObject*, UObject*, const wchar_t*, bool, uintptr_t, void*)>(DoSpoofCall)(Class, Outer, Name, ExactClass, SpoofCode, (void*)(GameBase + Offsets::StaticFindObject));
	}

	static UObject* FindObject(const wchar_t* Name, UObject* Outer = nullptr)
	{
		return StaticFindObject(nullptr, Outer, Name, false);
	}
};

class UFont : public UObject
{
public:

};

UObject* KismetMathLibrary = nullptr;
UObject* KismetMaterialLibrary = nullptr;
UObject* GameplayStatics = nullptr;
UObject* FortKismetLibrary = nullptr;
UObject* KismetSystemLibrary = nullptr;
UObject* KismetStringLibrary = nullptr;
UFont* MediumFont = 0;

class UEngine : public UObject
{
public:
	UFont* MediumFont()
	{
		return *(UFont**)(this + 0x70); 
	}

	UFont* SmallFont()
	{
		return *(UFont**)(this + 0x50);
	}
};

class UCanvas : public UObject
{
public:
	float ClipX()
	{
		return *(float*)(this + 0x30);
	}

	float ClipY()
	{
		return *(float*)(this + 0x34);
	}

	void K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, float Thickness, FLinearColor RenderColor)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Canvas.K2_DrawLine");

		struct {
			FVector2D ScreenPositionA;
			FVector2D ScreenPositionB;
			float Thickness;
			FLinearColor RenderColor;
		} Params = { ScreenPositionA, ScreenPositionB, Thickness, RenderColor };

		this->ProcessEvent(Function, &Params);
	}

	void K2_DrawText(FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, bool bCentreX, bool bCentreY, bool bOutlined)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Canvas.K2_DrawText");

		struct {
			UObject* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} Params = { MediumFont, RenderText, ScreenPosition, Scale, RenderColor, Kerning, FLinearColor(), FVector2D(), bCentreX, bCentreY, bOutlined, FLinearColor(0.f, 0.f, 0.f, 1.f) };

		this->ProcessEvent(Function, &Params);
	}
};

class UActorComponent : public UObject
{
public:

};

class USceneComponent : public UActorComponent
{
public:
	FRotator RelativeRotation()
	{
		return *(FRotator*)(this + 0x138);
	}
};

class AActor : public UObject
{
public:
	USceneComponent* RootComponent()
	{
		return *(USceneComponent**)(this + 0x198);
	}

	float GetActorTimeDilation()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Actor.GetActorTimeDilation");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool K2_SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Actor.K2_SetActorLocationAndRotation");

		struct {
			FVector NewLocation;
			FRotator NewRotation;
			bool bSweep;
			FHitResult SweepHitResult;
			bool bTeleport;
			bool ReturnValue;
		} Params = { NewLocation, NewRotation, false, FHitResult(), true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector K2_GetActorLocation()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Actor.K2_GetActorLocation");

		struct {
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator K2_GetActorRotation()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Actor.K2_GetActorRotation");

		struct {
			FRotator ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetGameTimeSinceCreation()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Actor.GetGameTimeSinceCreation");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class APlayerCameraManager : public AActor
{
public:
	float GetFOVAngle()
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerCameraManager.GetFOVAngle");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetCameraLocation()
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerCameraManager.GetCameraLocation");

		struct {
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator GetCameraRotation()
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerCameraManager.GetCameraRotation");

		struct {
			FRotator ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class APlayerState : public AActor
{
public:
	FString GetPlayerName()
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerState.GetPlayerName");

		struct {
			FString ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UMaterial : public UObject
{
public:

};

class UMaterialInterface : public UObject
{
public:
	UMaterial* GetMaterial()
	{
		if (auto VTable = *(void***)this)
		{
			return reinterpret_cast<UMaterial * (__cdecl*)(UMaterialInterface*, uintptr_t, void*)>(DoSpoofCall)(this, SpoofCode, VTable[Offsets::GetMaterial]);
		}

		return nullptr;
	}

	EBlendMode GetBlendMode()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MaterialInterface.GetBlendMode");

		struct {
			EBlendMode ReturnValue;
		} Params = {  };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UMaterialInstance : public UMaterialInterface
{
public:
	TArray<FScalarParameterValue> ScalarParameterValues()
	{
		return *(TArray<FScalarParameterValue>*)(this + 0x140);
	}

	TArray<FVectorParameterValue> VectorParameterValues()
	{
		return *(TArray<FVectorParameterValue>*)(this + 0x150);
	}
};

class UPrimitiveComponent : public USceneComponent
{
public:
	void SetCustomDepthStencilValue(int Value)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.SetCustomDepthStencilValue");

		struct {
			int Value;
		} Params = { Value };

		this->ProcessEvent(Function, &Params);
	}

	void SetRenderCustomDepth(bool bValue)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.SetRenderCustomDepth");

		struct {
			bool bValue;
		} Params = { bValue };

		this->ProcessEvent(Function, &Params);
	}

	UMaterialInterface* GetMaterial(int32_t ElementIndex)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.GetMaterial");

		struct {
			UMaterialInterface* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int32_t GetNumMaterials()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.GetNumMaterials");

		struct {
			int32_t ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsMaterialSlotNameValid(FName MaterialSlotName)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.IsMaterialSlotNameValid");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	TArray<FName> GetMaterialSlotNames()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.GetMaterialSlotNames");

		struct {
			TArray<FName> ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetMaterial(int32_t ElementIndex, UMaterialInterface* Material)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.SetMaterial");

		struct {
			int32_t ElementIndex;
			UMaterialInterface* Material;
		} Params = { ElementIndex, Material };

		this->ProcessEvent(Function, &Params);
	}

	void SetMaterialByName(FName MaterialSlotName, UMaterialInterface* Material)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PrimitiveComponent.SetMaterialByName");

		struct {
			FName MaterialSlotName;
			UMaterialInterface* Material;
		} Params = { MaterialSlotName, Material };

		this->ProcessEvent(Function, &Params);
	}
};

class UMeshComponent : public UPrimitiveComponent
{
public:
	void SetOverlayMaterial(UMaterialInterface* NewOverlayMaterial)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MeshComponent.SetOverlayMaterial");

		struct
		{
			UMaterialInterface* NewOverlayMaterial;
		} Params = { NewOverlayMaterial };

		this->ProcessEvent(Function, &Params);
	}

	TArray<UMaterialInterface*> GetMaterials()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MeshComponent.GetMaterials");

		struct
		{
			TArray<UMaterialInterface*> ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class USkinnedMeshComponent : public UMeshComponent
{
public:
	FMatrix GetBoneMatrix(int BoneIndex)
	{
		FMatrix OutMatrix;
		reinterpret_cast<FMatrix* (__cdecl*)(USkinnedMeshComponent*, FMatrix*, int, uintptr_t, void*)>(DoSpoofCall)(this, &OutMatrix, BoneIndex, SpoofCode, (void*)(GameBase + Offsets::GetBoneMatrix));

		return OutMatrix;
	}

	FVector GetBoneLocation(int BoneIndex)
	{
		FMatrix BoneMatrix = this->GetBoneMatrix(BoneIndex);

		return FVector(BoneMatrix.WPlane.X, BoneMatrix.WPlane.Y, BoneMatrix.WPlane.Z);
	}
};

class USkeletalMeshComponent : public USkinnedMeshComponent
{
public:

};

class UMaterialInstanceDynamic : public UMaterialInstance
{
public:
	void SetVectorParameterValue(FName ParameterName, FLinearColor Value)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MaterialInstanceDynamic.SetVectorParameterValue");

		struct {
			FName ParameterName;
			FLinearColor Value;
		} Params = { ParameterName, Value };

		this->ProcessEvent(Function, &Params);
	}

	FLinearColor K2_GetVectorParameterValue(FName ParameterName)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MaterialInstanceDynamic.K2_GetVectorParameterValue");

		struct {
			FName ParameterName;
			FLinearColor ReturnValue;
		} Params = { ParameterName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetScalarParameterValue(FName ParameterName, float Value)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MaterialInstanceDynamic.SetScalarParameterValue");

		struct {
			FName ParameterName;
			float Value;
		} Params = { ParameterName, Value };

		this->ProcessEvent(Function, &Params);
	}

	float K2_GetScalarParameterValue(FName ParameterName)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MaterialInstanceDynamic.K2_GetScalarParameterValue");

		struct {
			FName ParameterName;
			float ReturnValue;
		} Params = { ParameterName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UCharacterMovementComponent : public UObject
{
public:

};

class UFortItemDefinition
{
public:
	FText DisplayName()
	{
		return *(FText*)(this + 0x90);
	}

	EFortItemTier Tier()
	{
		return *(EFortItemTier*)(this + 0x73);
	}
};

class UFortWeaponItemDefinition : public UFortItemDefinition
{
public:

};

class AFortWeapon : public AActor
{
public:
	UFortWeaponItemDefinition* WeaponData()
	{
		return *(UFortWeaponItemDefinition**)(this + 0x4a8);
	}

	int AmmoCount()
	{
		return *(int*)(this + 0xdcc);
	}

	float LastFireAbilityTime()
	{
		return *(float*)(this + 0x1084);
	}

	float GetReloadProgress()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetReloadProgress");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetFiringRate()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetFiringRate");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetReloadTime()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetReloadTime");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsProjectileWeapon()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.IsProjectileWeapon");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetMuzzleLocation()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetMuzzleLocation");

		struct {
			int PatternIndex;
			FVector ReturnValue;
		} Params{ 0 };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetBulletsPerClip()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetBulletsPerClip");

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsReloading()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.IsReloading");

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetMagazineAmmoCount()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetMagazineAmmoCount");

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetRemainingAmmo()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetRemainingAmmo");

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetChargePercent()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetChargePercent");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetProjectileSpeed(float ChargePercent)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortWeapon.GetProjectileSpeed");

		struct {
			float ChargePercent;
			float ReturnValue;
		} Params = { ChargePercent };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortWeaponRanged : public AFortWeapon
{
public:

};

class AFortAthenaVehicle : public AActor
{
public:

};

class AFortPlayerMarkerBase : public AActor {
public:

};

class APawn : public AActor
{
public:
	APlayerState* PlayerState()
	{
		return *(APlayerState**)(this + 0x2b0);
	}
};

class ACharacter : public APawn
{
public:
	USkeletalMeshComponent* Mesh()
	{
		return *(USkeletalMeshComponent**)(this + 0x318);
	}

	UCharacterMovementComponent* CharacterMovement()
	{
		return *(UCharacterMovementComponent**)(this + 0x320);
	}

	bool CanJump()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"Character.CanJump");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPawn : public ACharacter
{
public:
	AFortWeapon* CurrentWeapon()
	{
		return *(AFortWeapon**)(this + 0xa20);
	}

	bool IsDead()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortPawn.IsDead");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsDBNO()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortPawn.IsDBNO");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	char GetTeam()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortPawn.GetTeam");

		struct {
			char ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPlayerPawn : public AFortPawn
{
public:
	AFortAthenaVehicle* CurrentVehicle()
	{
		return *(AFortAthenaVehicle**)(this + 0x2840);
	}

	bool IsInVehicle()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortPlayerPawn.IsInVehicle");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPlayerPawnAthena : public AFortPlayerPawn
{
public:

};

class APlayerPawn_Athena_Generic_Parent_C : public AFortPlayerPawnAthena
{
public:
	TArray<USkeletalMeshComponent*> SkeletalMeshes()
	{
		return *(TArray<USkeletalMeshComponent*>*)(this + 0x5b70);
	}

	TArray<UMaterialInstanceDynamic*> PawnMaterials_ALL()
	{
		return *(TArray<UMaterialInstanceDynamic*>*)(this + 0x5b80);
	}
};

class APlayerPawn_Athena_Generic_C : public APlayerPawn_Athena_Generic_Parent_C
{
public:

};

class APlayerPawn_Athena_C : public APlayerPawn_Athena_Generic_C
{
public:
	void ApplyPawnHighlight(UObject* Source, FPawnHighlight HitGlow)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerPawn_Athena_C.ApplyPawnHighlight");

		struct {
			UObject* Source;
			FPawnHighlight HitGlow;
		} Params = { Source, HitGlow };

		this->ProcessEvent(Function, &Params);
	}
};

class AFortPickup : public AActor
{
public:
	UFortItemDefinition* PrimaryPickupItemEntry_ItemDefinition()
	{
		return *(UFortItemDefinition**)((this + 0x350) + 0x18);
	}
};

class ABuildingContainer : public AActor
{
public:

};


class UMovementComponent : public UActorComponent
{
public:
	void StopMovementImmediately()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"MovementComponent.StopMovementImmediately");

		struct {

		} Params = { };

		this->ProcessEvent(Function, &Params);
	}
};

class UFortProjectileMovementComponent : public UMovementComponent
{
public:
	void MoveInterpolationTarget(FVector NewLocation, FRotator NewRotation)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"ProjectileMovementComponent.MoveInterpolationTarget");

		struct {
			FVector NewLocation;
			FRotator NewRotation;
		} Params = { NewLocation, NewRotation };

		this->ProcessEvent(Function, &Params);
	}

	bool IsInterpolationComplete()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"ProjectileMovementComponent.IsInterpolationComplete");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortProjectileBase : public AActor
{
public:
	UFortProjectileMovementComponent* ProjectileMovementComponent()
	{
		return *(UFortProjectileMovementComponent**)(this + 0x778);
	}

	AFortWeapon* GetOwnerWeapon()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortProjectileBase.GetOwnerWeapon");

		struct {
			AFortWeapon* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class ABuildingWeakSpot : public AActor
{
public:

};

class AFortAthenaSupplyDrop : public AActor
{
public:

};

class ABuildingTrap : public AActor
{
public:
	bool HasDurability()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"BuildingTrap.HasDurability");

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetReloadTime()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"BuildingTrap.GetReloadTime");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetDurabilityPercent()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"BuildingTrap.GetDurabilityPercent");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetDamageDelay()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"BuildingTrap.GetDamageDelay");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetArmTime()
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"BuildingTrap.GetArmTime");

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AController : public AActor
{
public:
	APlayerState* PlayerState()
	{
		return *(APlayerState**)(this + 0x298);
	}
};

class APlayerController : public AController
{
public:
	APlayerCameraManager* PlayerCameraManager()
	{
		return *(APlayerCameraManager**)(this + 0x348);
	}

	APlayerPawn_Athena_C* AcknowledgedPawn()
	{
		return *(APlayerPawn_Athena_C**)(this + 0x338);
	}

	float InputYawScale()
	{
		return *(float*)(this + 0x538);
	}

	float InputPitchScale()
	{
		return *(float*)(this + 0x53c);
	}

	void AddYawInput(float Val)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerController.AddYawInput");

		struct
		{
			float Val;
		} params = { Val };

		this->ProcessEvent(Function, &params);
	}

	void AddPitchInput(float Val)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerController.AddPitchInput");

		struct
		{
			float Val;
		} params = { Val };

		this->ProcessEvent(Function, &params);
	}

	bool ProjectWorldLocationToScreen(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		static UObject* Function;
		if (!Function) Function = FindObject(L"PlayerController.ProjectWorldLocationToScreen");
		struct {
			FVector WorldLocation;
			FVector2D ScreenLocation;
			bool bPlayerViewportRelative;
			bool ReturnValue;
		} Params = { WorldLocation, FVector2D(), false };

		this->ProcessEvent(Function, &Params);

		if (ScreenLocation) *ScreenLocation = FVector2D(int(Params.ScreenLocation.X), int(Params.ScreenLocation.Y));

		return Params.ReturnValue && Params.ScreenLocation;
	}

	FVector2D GetMousePosition()
	{
		static UObject* Function;
		if (!Function) Function = FindObject(L"PlayerController.GetMousePosition");

		struct
		{
			float LocationX;
			float LocationY;
			bool ReturnValue;
		} params = { };

		this->ProcessEvent(Function, &params);

		return FVector2D(double(params.LocationX), double(params.LocationY));
	}

	bool IsInputKeyDown(FKey key)
	{
		static UObject* Function;
		if (!Function) Function = FindObject(L"PlayerController.IsInputKeyDown");

		struct
		{
			FKey key;
			bool ReturnValue;
		} params = { key };

		this->ProcessEvent(Function, &params);

		return params.ReturnValue;
	}

	bool WasInputKeyJustPressed(FKey key)
	{
		static UObject* Function;
		if (!Function) Function = FindObject(L"PlayerController.WasInputKeyJustPressed");

		struct
		{
			FKey key;
			bool ReturnValue;
		} params = { key };

		this->ProcessEvent(Function, &params);

		return params.ReturnValue;
	}

	void FOV(float NewFOV)
	{
		static UObject* Function;
		if (!Function) Function = FindObject(L"PlayerController.FOV");

		struct {
			float NewFOV;
		} Params = { NewFOV };

		this->ProcessEvent(Function, &Params);
	}

	void ClientReturnToMainMenu(FString& ReturnReason)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerController.ClientReturnToMainMenu");

		struct {
			FString ReturnReason;
		} Params = { ReturnReason };

		this->ProcessEvent(Function, &Params);
	}

	void SetMouseLocation(int32_t X, int32_t Y)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"PlayerController.SetMouseLocation");

		struct {
			int32_t X;
			int32_t Y;
		} Params = { X, Y };

		this->ProcessEvent(Function, &Params);
	}

	bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad = false)
	{
		if (auto VTable = *(void***)this)
		{
			return reinterpret_cast<bool(__cdecl*)(APlayerController*, FKey, EInputEvent, float, bool, uintptr_t, void*)>(DoSpoofCall)(this, Key, EventType, AmountDepressed, bGamepad, SpoofCode, VTable[Offsets::InputKey]);
		}

		return false;
	}
};

class UGameViewportClient : public UObject
{
public:

};

class UPlayer : public UObject
{
public:
	APlayerController* PlayerController()
	{
		return *(APlayerController**)(this + 0x30);
	}
};

class UFortClientSettingsRecord : public UObject
{
public:
	float HUDScale()
	{
		return *(float*)(this + 0x490);
	}
};

class ULocalPlayer : public UPlayer
{
public:
	UGameViewportClient* ViewportClient()
	{
		return *(UGameViewportClient**)(this + 0x78);
	}

	UFortClientSettingsRecord* ClientSettingsRecord()
	{
		return *(UFortClientSettingsRecord**)(this + 0x390);
	}
};

class UGameInstance : public UObject
{
public:
	TArray <ULocalPlayer*> LocalPlayers()
	{
		return *(TArray <ULocalPlayer*>*)(this + 0x38);
	}
};

class UWorld : public UObject
{
public:
	UGameInstance* OwningGameInstance()
	{
		return *(UGameInstance**)(this + 0x1d0);
	}

	static UWorld* GetWorld()
	{
		return *(UWorld**)(GameBase + Offsets::GWorld);
	}
};

class UFortKismetLibrary : public UObject
{
public:
	static bool CheckLineOfSightToActorWithChannel(FVector SourcePos, AActor* Target, ECollisionChannel TargetFilterChannel, AActor* Source)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortKismetLibrary.CheckLineOfSightToActorWithChannel");

		struct {
			FVector SourcePos;
			AActor* Target;
			ECollisionChannel TargetFilterChannel;
			AActor* Source;
			bool ReturnValue;
		} Params = { SourcePos, Target, TargetFilterChannel, Source };

		FortKismetLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static bool CheckPlayerLineOfSightToPawn(AFortPlayerPawn* SourcePawn, AFortPawn* TargetPawn)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"FortKismetLibrary.CheckPlayerLineOfSightToPawn");

		struct {
			AFortPlayerPawn* SourcePawn;
			AFortPawn* TargetPawn;
			bool ReturnValue;
		} Params = { SourcePawn, TargetPawn };

		FortKismetLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetMathLibrary : public UObject
{
public:
	static double sin(double A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.sin");

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double cos(double A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.cos");

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double acos(double A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.acos");

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double tan(double A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.tan");

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Atan2(double Y, double X)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.Atan2");

		struct {
			double Y;
			double X;
			double ReturnValue;
		} Params = { Y, X };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double abs(double A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.abs");

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double sqrt(double A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.sqrt");

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static int32 RandomIntegerInRange(int32 Min, int32 Max)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.RandomIntegerInRange");

		struct {
			int32 Min;
			int32 Max;
			int32 ReturnValue;
		} Params = { Min, Max };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static float DegreesToRadians(float A)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.DegreesToRadians");

		struct {
			float A;
			float ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FVector GetForwardVector(const FRotator& InRot)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.GetForwardVector");

		struct {
			FRotator InRot;
			FVector ReturnValue;
		} Params = { InRot };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator FindLookAtRotation(FVector& Start, FVector& Target)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.FindLookAtRotation");

		struct {
			FVector Start;
			FVector Target;
			FRotator ReturnValue;
		} Params = { Start, Target };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Vector_Distance(FVector v1, FVector v2)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.Vector_Distance");

		struct {
			FVector v1;
			FVector v2;
			double ReturnValue;
		} Params = { v1, v2 };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Vector_Distance2D(FVector2D v1, FVector2D v2)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.Vector_Distance2D");

		struct {
			FVector v1;
			FVector v2;
			double ReturnValue;
		} Params = { FVector(v1.X, v1.Y, 0.0), FVector(v2.X, v2.Y, 0.0) };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FVector GetReflectionVector(FVector Direction, FVector SurfaceNormal)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.GetReflectionVector");

		struct {
			FVector Direction;
			FVector SurfaceNormal;
			FVector ReturnValue;
		} Params = { Direction, SurfaceNormal };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator RInterpTo_Constant(FRotator& Current, FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.RInterpTo_Constant");

		struct {
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator RInterpTo(FRotator& Current, FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.RInterpTo");

		struct {
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double FInterpTo(double Current, double Target, double DeltaTime, double Speed)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetMathLibrary.RInterpTo");

		struct
		{
			double Current;
			double Target;
			double DeltaTime;
			double Speed;
			double ReturnValue;
		} Params = { Current, Target, DeltaTime, Speed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UGameplayStatics : public UObject
{
public:
	static TArray<UObject*> GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"GameplayStatics.GetAllActorsOfClass");

		struct {
			UObject* WorldContextObject;
			UObject* ActorClass;
			TArray<UObject*> OutActors;
		} Params = { WorldContextObject, ActorClass };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.OutActors;
	}

	static bool ObjectIsA(UObject* Object, UObject* ObjectClass)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"GameplayStatics.ObjectIsA");

		struct {
			UObject* Object;
			UObject* ObjectClass;
			bool ReturnValue;
		} Params = { Object, ObjectClass };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double GetWorldDeltaSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"GameplayStatics.GetWorldDeltaSeconds");

		struct {
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetSystemLibrary : public UObject
{
public:
	static double GetGameTimeInSeconds(UObject* WorldContextObject)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetSystemLibrary.GetGameTimeInSeconds");

		struct {
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static bool LineTraceSingle(UObject* WorldContextObject, FVector Start, FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetSystemLibrary.LineTraceSingle");

		struct {
			UObject* WorldContextObject;
			FVector Start;
			FVector End;
			char TraceChannel;
			bool bTraceComplex;
			TArray<AActor*> ActorsToIgnore;
			char DrawDebugType;
			FHitResult OutHit;
			bool bIgnoreSelf;
			FLinearColor TraceColor;
			FLinearColor TraceHitColor;
			float DrawTime;
			bool ReturnValue;
		} Params = { WorldContextObject, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, FLinearColor(0.f, 0.f, 0.f, 1.f), FLinearColor(0.f, 0.f, 0.f, 1.f), 0.1f };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString GetObjectName(UObject* Object)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetSystemLibrary.GetObjectName");

		struct {
			UObject* Object;
			FString ReturnValue;
		} Params = { Object };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString GetPathName(UObject* Object)
	{
		UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetSystemLibrary.GetPathName");

		struct {
			UObject* Object;
			FString ReturnValue;
		} Params = { Object };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetStringLibrary : public UObject
{
public:
	static bool Contains(FString SearchIn, FString Substring, bool bUseCase, bool bSearchFromEnd)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetStringLibrary.Contains");

		struct {
			FString SearchIn;
			FString Substring;
			bool bUseCase;
			bool bSearchFromEnd;
			bool ReturnValue;
		} Params = { SearchIn, Substring, bUseCase, bSearchFromEnd };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FName Conv_StringToName(FString InString)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetStringLibrary.Conv_StringToName");

		struct {
			FString InString;
			FName ReturnValue;
		} Params = { InString };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString Conv_NameToString(FName InName)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetStringLibrary.Conv_NameToString");

		struct {
			FName InName;
			FString ReturnValue;
		} Params = { InName };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString BuildString_Double(FString AppendTo, FString Prefix, double InDouble, FString Suffix)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetStringLibrary.BuildString_Double");

		struct
		{
			FString AppendTo;
			FString Prefix;
			double InDouble;
			FString Suffix;
			FString ReturnValue;
		} params = { AppendTo, Prefix, InDouble, Suffix };

		KismetStringLibrary->ProcessEvent(Function, &params);

		return params.ReturnValue;
	}

	static FString BuildString_Float(FString AppendTo, FString Prefix, float InFloat, FString Suffix)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetStringLibrary.BuildString_Float");

		struct
		{
			FString AppendTo;
			FString Prefix;
			float InFloat;
			FString Suffix;
			FString ReturnValue;
		} params = { AppendTo, Prefix, InFloat, Suffix };

		KismetStringLibrary->ProcessEvent(Function, &params);

		return params.ReturnValue;
	}

	static FString BuildString_Int(FString AppendTo, FString Prefix, int InInt, FString Suffix, bool AllCrypted = false)
	{
		static UObject* Function = 0;
		if (!Function) Function = FindObject(L"KismetStringLibrary.BuildString_Int");

		struct {
			FString AppendTo;
			FString Prefix;
			int InInt;
			FString Suffix;
			FString ReturnValue;
		} Params = { AppendTo, Prefix, InInt, Suffix };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

namespace Custom
{
	FLinearColor GetColorByTier(EFortItemTier Tier)
	{
		if (Tier == EFortItemTier::I)
			return FLinearColor(0.3f, 0.3f, 0.3f, 1.f);
		else if (Tier == EFortItemTier::II)
			return FLinearColor(0.3f, 0.7f, 0.3f, 1.f);
		else if (Tier == EFortItemTier::III)
			return FLinearColor(0.3f, 0.3f, 0.7f, 1.f);
		else if (Tier == EFortItemTier::IV)
			return FLinearColor(0.7f, 0.3f, 0.7f, 1.f);
		else if (Tier == EFortItemTier::V)
			return FLinearColor(1.f, 1.f, 0.f, 1.f);
		else if (Tier == EFortItemTier::VI)
			return FLinearColor(1.f, 0.f, 0.f, 1.f);

		return FLinearColor(1.f, 1.f, 1.f, 1.f);
	}

	void AddInput(UWorld* GWorld, APlayerController* PlayerController, FRotator CameraRotation, FRotator TargetRotation, bool Interpolate, bool Constant, double Speed)
	{
		double WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(GWorld);

		if (Interpolate && Constant)
			TargetRotation = UKismetMathLibrary::RInterpTo_Constant(CameraRotation, TargetRotation, float(WorldDeltaSeconds), float(Speed));
		else if (Interpolate)
			TargetRotation = UKismetMathLibrary::RInterpTo(CameraRotation, TargetRotation, float(WorldDeltaSeconds), float(Speed));

		TargetRotation.Pitch = (TargetRotation.Pitch - CameraRotation.Pitch) / double(PlayerController->InputPitchScale());
		TargetRotation.Yaw = (TargetRotation.Yaw - CameraRotation.Yaw) / double(PlayerController->InputYawScale());

		PlayerController->AddPitchInput(float(TargetRotation.Pitch));
		PlayerController->AddYawInput(float(TargetRotation.Yaw));
	}

	FVector PredictProjectile(UWorld* World, FVector StartLocation, FVector ForwardVector, double Gravity, double InitialSpeed, double TimeStep, double SlideMultiplier = 1.0)
	{
		TArray<FVector> PredictedPath;
		TArray<AActor*> IgnoredActors;
		FVector CurrentLocation = StartLocation;
		FVector CurrentVelocity = ForwardVector * InitialSpeed;

		FHitResult HitResult;
		FVector NextLocation = CurrentLocation + (CurrentVelocity * TimeStep);

		// Perform line trace to check for collisions
		bool bHit = UKismetSystemLibrary::LineTraceSingle(World, CurrentLocation, NextLocation, ETraceTypeQuery::TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult, false);
		if (bHit)
		{
			FVector ReflectVector = UKismetMathLibrary::GetReflectionVector(CurrentVelocity, HitResult.Normal);
			FVector SlideVector = ReflectVector * SlideMultiplier;
			CurrentVelocity = SlideVector;

			// Calculate the new location after the bounce
			NextLocation = CurrentLocation + (CurrentVelocity * TimeStep);

			// Update the CurrentLocation to the new location after the bounce
			CurrentLocation = NextLocation;

			// Add the bounce location to the predicted path
			return HitResult.Location;
		}
		else
		{
			// Update the CurrentLocation to the next location
			CurrentLocation = NextLocation;

			// Apply gravity to the velocity
			CurrentVelocity += FVector(0.0, 0.0, -Gravity) * TimeStep;

			return NextLocation;
		}
	}

	bool InRect(double x, double y, double radius, double coord_x, double coord_y)
	{
		return coord_x >= x - radius && coord_x <= x + radius &&
			coord_y >= y - radius && coord_y <= y + radius;
	}

	bool InCircle(double x, double y, double radius, double coord_x, double coord_y)
	{
		if (InRect(x, coord_y, radius, x, coord_y))
		{
			double dx = x - coord_x;
			double dy = y - coord_y;
			dx *= dx;
			dy *= dy;
			double distance_squared = dx + dy;
			double radius_squared = radius * radius;
			return distance_squared <= radius_squared;
		} return false;
	}
}