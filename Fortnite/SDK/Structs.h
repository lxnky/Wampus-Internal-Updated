#pragma once

#define MergeInternal(A, B) A##B
#define Merge(A, B) MergeInternal(A, B)
#define Pad(Size) unsigned char Merge(_pad_, __COUNTER__)[Size]

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#define M_PI 3.14159265358979323846
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308
#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F
#define INT_MAX 2147483647
#define INT_MIN (-2147483647 - 1)

class FUObjectItem
{
public:
	void* Object;
	int32_t Flags;
	int32_t ClusterIndex;
	int32_t SerialNumber;

	enum class ObjectFlags : int32_t
	{
		None = 0,
		Native = 1 << 25,
		Async = 1 << 26,
		AsyncLoading = 1 << 27,
		Unreachable = 1 << 28,
		PendingKill = 1 << 29,
		RootSet = 1 << 30,
		NoStrongReference = 1 << 31
	};

	inline bool IsUnreachable() const
	{
		return !!(Flags & static_cast<std::underlying_type_t<ObjectFlags>>(ObjectFlags::Unreachable));
	}

	inline bool IsPendingKill() const
	{
		return !!(Flags & static_cast<std::underlying_type_t<ObjectFlags>>(ObjectFlags::PendingKill));
	}
};

class FChunkedFixedUObjectArray
{
public:
	inline int32_t Num() const
	{
		return NumElements;
	}

	bool IsValidIndex(int Index) const
	{
		return Index < NumElements && Index >= 0;
	}

	FUObjectItem* GetObjectPtr(int Index)
	{
		int ChunkIndex = Index / NumElementsPerChunk;
		int WithinChunkIndex = Index % NumElementsPerChunk;

		if (!IsValidIndex(Index)) return nullptr;

		if (ChunkIndex >= NumChunks) return nullptr;

		if (Index > MaxElements) return nullptr;

		FUObjectItem* Chunk = &Objects[ChunkIndex];
		if (!Chunk) return nullptr;

		return Chunk + WithinChunkIndex;
	}

	inline FUObjectItem* GetItemByIndex(int32_t index) const
	{
		if (index < NumElements)
		{
			return &Objects[index];
		}
		return nullptr;
	}
private:
	enum
	{
		NumElementsPerChunk = 64 * 1024,
	};

	FUObjectItem* Objects;
	FUObjectItem* PreAllocatedObjects;
	int32_t MaxElements;
	int32_t NumElements;
	int32_t MaxChunks;
	int32_t NumChunks;
}; typedef FChunkedFixedUObjectArray TUObjectArray;

TUObjectArray* GObjects = nullptr;

static TUObjectArray* GetObjects()
{
	return *(TUObjectArray**)(GameBase + Offsets::GObjects);
}

struct FWeakObjectPtr
{
public:
	inline bool SerialNumbersMatch(FUObjectItem* ObjectItem) const
	{
		return ObjectItem->SerialNumber == ObjectSerialNumber;
	}

	bool IsValid() const
	{
		if (ObjectSerialNumber == 0 && ObjectIndex < 0)
		{
			return false;
		}

		auto ObjectItem = GObjects->GetItemByIndex(ObjectIndex);
		if (!ObjectItem)
		{
			return false;
		}

		if (!SerialNumbersMatch(ObjectItem))
		{
			return false;
		}

		return !(ObjectItem->IsUnreachable() || ObjectItem->IsPendingKill());
	}

	void* Get() const
	{
		if (IsValid())
		{
			FUObjectItem* ObjectItem = GObjects->GetItemByIndex(ObjectIndex);
			if (ObjectItem)
			{
				return ObjectItem->Object;
			}
		}

		return nullptr;
	}

	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};

template<class ObjectType>
struct TWeakObjectPtr : private FWeakObjectPtr
{
public:
	inline ObjectType* Get() const
	{
		return (ObjectType*)FWeakObjectPtr::Get();
	}

	inline ObjectType& operator*() const
	{
		return *Get();
	}

	inline ObjectType* operator->() const
	{
		return Get();
	}

	inline bool IsValid() const
	{
		return FWeakObjectPtr::IsValid();
	}
};

template <class ArrayType>
struct TArray
{
	TArray()
	{
		this->Data = nullptr;
		this->Count = this->Max = 0;
	};

	ArrayType* Data;
	int Count, Max;

	ArrayType& operator[](int i)
	{
		return this->Data[i];
	};

	void PushBack(ArrayType Member)
	{
		this->Count += 1;
		this->Max += 1;

		//Allocate new array
		ArrayType* NewAllocation = reinterpret_cast<ArrayType * (__cdecl*)(uintptr_t, int, uintptr_t, void*)>(DoSpoofCall)(this->Count * sizeof(ArrayType), 0x10, SpoofCode, (void*)(GameBase + Offsets::Malloc));

		//Fill with old data
		for (int i = 0; i < this->Count - 1; i++)
		{
			NewAllocation[i] = this->Data[i];
		}

		NewAllocation[this->Count - 1] = Member;

		//Free old array
		if (this->Data)
		{
			reinterpret_cast<void* (__cdecl*)(void*, uintptr_t, void*)>(DoSpoofCall)(this->Data, SpoofCode, (void*)(GameBase + Offsets::Free));
		}

		//Set new data pointer
		this->Data = NewAllocation;
	}

	void Clear()
	{
		//Free old array
		reinterpret_cast<void* (__cdecl*)(void*, uintptr_t, void*)>(DoSpoofCall)(this->Data, SpoofCode, (void*)(GameBase + Offsets::Free));

		this->Data = nullptr;

		this->Count = this->Max = 0;
	}

	int Size()
	{
		return this->Count;
	}

	bool Valid(int i)
	{
		return bool(i < this->Count);
	}
};

struct FString : private TArray<wchar_t>
{
	FString() { };

	FString(const wchar_t* Other)
	{
		this->Max = this->Count = *Other ? static_cast<int>(__wcslen(Other)) + 1 : 0;

		if (this->Count)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	FString(const wchar_t* Other, int Size)
	{
		this->Max = this->Count = Size;

		if (this->Count)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	operator bool() { return bool(this->Data); }

	wchar_t* c_str()
	{
		return this->Data;
	}

	int Size()
	{
		return this->Count;
	}
};

struct FVector
{
	FVector() : X(), Y(), Z() { }
	FVector(double X, double Y, double Z) : X(X), Y(Y), Z(Z) { }

	FVector operator + (const FVector& other) const { return { this->X + other.X, this->Y + other.Y, this->Z + other.Z }; }
	FVector operator - (const FVector& other) const { return { this->X - other.X, this->Y - other.Y, this->Z - other.Z }; }
	FVector operator * (double offset) const { return { this->X * offset, this->Y * offset, this->Z * offset }; }
	FVector operator / (double offset) const { return { this->X / offset, this->Y / offset, this->Z / offset }; }

	FVector& operator *= (const double other) { this->X *= other; this->Y *= other; this->Z *= other; return *this; }
	FVector& operator /= (const double other) { this->X /= other; this->Y /= other; this->Z /= other; return *this; }

	FVector& operator = (const FVector& other) { this->X = other.X; this->Y = other.Y; this->Z = other.Z; return *this; }
	FVector& operator += (const FVector& other) { this->X += other.X; this->Y += other.Y; this->Z += other.Z; return *this; }
	FVector& operator -= (const FVector& other) { this->X -= other.X; this->Y -= other.Y; this->Z -= other.Z; return *this; }
	FVector& operator *= (const FVector& other) { this->X *= other.X; this->Y *= other.Y; this->Z *= other.Z; return *this; }
	FVector& operator /= (const FVector& other) { this->X /= other.X; this->Y /= other.Y; this->Z /= other.Z; return *this; }

	operator bool() { return bool(this->X || this->Y || this->Z); }
	friend bool operator == (const FVector& a, const FVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
	friend bool operator != (const FVector& a, const FVector& b) { return !(a == b); }

	double Dot(const FVector& V) { return X * V.X + Y * V.Y + Z * V.Z; }

	double X, Y, Z;
};

struct FVector2D
{
	FVector2D() : X(), Y() { }
	FVector2D(double X, double Y) : X(X), Y(Y) { }

	FVector2D operator + (const FVector2D& other) const { return { this->X + other.X, this->Y + other.Y }; }
	FVector2D operator - (const FVector2D& other) const { return { this->X - other.X, this->Y - other.Y }; }
	FVector2D operator * (double offset) const { return { this->X * offset, this->Y * offset }; }
	FVector2D operator / (double offset) const { return { this->X / offset, this->Y / offset }; }

	FVector2D& operator *= (const double other) { this->X *= other; this->Y *= other; return *this; }
	FVector2D& operator /= (const double other) { this->X /= other; this->Y /= other; return *this; }

	FVector2D& operator = (const FVector2D& other) { this->X = other.X; this->Y = other.Y; return *this; }
	FVector2D& operator += (const FVector2D& other) { this->X += other.X; this->Y += other.Y; return *this; }
	FVector2D& operator -= (const FVector2D& other) { this->X -= other.X; this->Y -= other.Y; return *this; }
	FVector2D& operator *= (const FVector2D& other) { this->X *= other.X; this->Y *= other.Y; return *this; }
	FVector2D& operator /= (const FVector2D& other) { this->X /= other.X; this->Y /= other.Y; return *this; }

	operator bool() { return bool(this->X || this->Y); }
	friend bool operator == (const FVector2D& A, const FVector2D& B) { return A.X == B.X && A.Y == A.Y; }
	friend bool operator != (const FVector2D& A, const FVector2D& B) { return !(A == B); }

	double X, Y;
};

struct FRotator
{
	FRotator() : Pitch(), Yaw(), Roll() { }
	FRotator(double Pitch, double Yaw, double Roll) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) { }

	FRotator operator + (const FRotator& other) const { return { this->Pitch + other.Pitch, this->Yaw + other.Yaw, this->Roll + other.Roll }; }
	FRotator operator - (const FRotator& other) const { return { this->Pitch - other.Pitch, this->Yaw - other.Yaw, this->Roll - other.Roll }; }
	FRotator operator * (double offset) const { return { this->Pitch * offset, this->Yaw * offset, this->Roll * offset }; }
	FRotator operator / (double offset) const { return { this->Pitch / offset, this->Yaw / offset, this->Roll / offset }; }

	FRotator& operator *= (const double other) { this->Pitch *= other; this->Yaw *= other; this->Roll *= other; return *this; }
	FRotator& operator /= (const double other) { this->Pitch /= other; this->Yaw /= other; this->Roll /= other; return *this; }

	FRotator& operator = (const FRotator& other) { this->Pitch = other.Pitch; this->Yaw = other.Yaw; this->Roll = other.Roll; return *this; }
	FRotator& operator += (const FRotator& other) { this->Pitch += other.Pitch; this->Yaw += other.Yaw; this->Roll += other.Roll; return *this; }
	FRotator& operator -= (const FRotator& other) { this->Pitch -= other.Pitch; this->Yaw -= other.Yaw; this->Roll -= other.Roll; return *this; }
	FRotator& operator *= (const FRotator& other) { this->Pitch *= other.Pitch; this->Yaw *= other.Yaw; this->Roll *= other.Roll; return *this; }
	FRotator& operator /= (const FRotator& other) { this->Pitch /= other.Pitch; this->Yaw /= other.Yaw; this->Roll /= other.Roll; return *this; }

	operator bool() { return bool(this->Pitch || this->Yaw || this->Roll); }
	friend bool operator == (const FRotator& a, const FRotator& b) { return a.Pitch == b.Pitch && a.Yaw == b.Yaw && a.Roll == b.Roll; }
	friend bool operator != (const FRotator& a, const FRotator& b) { return !(a == b); }

	FVector Euler() const
	{
		return FVector(Pitch, Yaw, Roll);
	}

	double Pitch, Yaw, Roll;
};

struct FLinearColor
{
	FLinearColor() : r(0.f), g(0.f), b(0.f), a(0.f) {}
	FLinearColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	operator bool() { return bool(this->r || this->g || this->b || this->a); }
	friend bool operator	== (const FLinearColor& a, const FLinearColor& b) { return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a; }

	float r, g, b, a;
};

struct FPlane : public FVector
{
	FPlane() : W() { }
	FPlane(double W) : W(W) { }

	double W;
};

struct FMatrix
{
	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() { }
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane) : XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) { }

	FPlane XPlane, YPlane, ZPlane, WPlane;
};

struct FName
{
	FName() : Index() { }
	FName(int Index) : Index(Index) { }

	operator bool() { return bool(this->Index); }
	friend bool operator	== (const FName& a, const FName& b) { return a.Index == b.Index; }
	friend bool operator	!= (const FName& a, const FName& b) { return !(a == b); }

	int Index;
	uint32_t Number;
};

class FTextData
{
public:
	char pad_0x0000[0x28];  //0x0000
	wchar_t* Name;          //0x28 
	__int32 Length;         //0x40 

};

struct FText {
	FTextData* Data;
	char UnknownData[0x10];

	wchar_t* Get() const {
		if (Data)
			return Data->Name;

		return nullptr;
	}
};

struct FKey
{
	FName KeyName;
	char Details[0x10]; // Pad_28B4
};

enum EBlendMode : uint8
{
	BLEND_Opaque = 0,
	BLEND_Masked = 1,
	BLEND_Translucent = 2,
	BLEND_Additive = 3,
	BLEND_Modulate = 4,
	BLEND_AlphaComposite = 5,
	BLEND_MAX = 6,
};

enum EDecalBlendMode
{
	DBM_Translucent,
	DBM_Stain,
	DBM_Normal,
	DBM_Emissive,
	DBM_DBuffer_ColorNormalRoughness,
	DBM_DBuffer_Color,
	DBM_DBuffer_ColorNormal,
	DBM_DBuffer_ColorRoughness,
	DBM_DBuffer_Normal,
	DBM_DBuffer_NormalRoughness,
	DBM_DBuffer_Roughness,
	DBM_DBuffer_Emissive,
	DBM_DBuffer_AlphaComposite,
	DBM_DBuffer_EmissiveAlphaComposite,
	DBM_Volumetric_DistanceFunction,
	DBM_AlphaComposite,
	DBM_AmbientOcclusion,
	DBM_MAX,
};

enum ETranslucencyLightingMode : uint8
{
	TLM_VolumetricNonDirectional = 0,
	TLM_VolumetricDirectional = 1,
	TLM_VolumetricPerVertexNonDirectional = 2,
	TLM_VolumetricPerVertexDirectional = 3,
	TLM_Surface = 4,
	TLM_SurfacePerPixelLighting = 5,
	TLM_MAX = 6,
};

enum ERendererStencilMask : uint8
{
	ERSM_Default = 0,
	ERSM_255 = 1,
	ERSM_1 = 2,
	ERSM_2 = 3,
	ERSM_4 = 4,
	ERSM_8 = 5,
	ERSM_16 = 6,
	ERSM_32 = 7,
	ERSM_64 = 8,
	ERSM_128 = 9,
	ERSM_MAX = 10,
};

struct FHitResult
{
public:
	int8 bBlockingHit : 1;                                  // Mask: 0x1, PropSize: 0x10x0(0x1)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 bStartPenetrating : 1;                             // Mask: 0x2, PropSize: 0x10x0(0x1)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 BitPad_113 : 6;                                    // Fixing Bit-Field Size  [ Dumper-7 ]
	uint8 Pad_A82[0x3];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	int32 FaceIndex;                                         // 0x4(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float Time;                                              // 0x8(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float Distance;                                          // 0xC(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector Location;                                          // 0x10(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector ImpactPoint;                                       // 0x1C(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector Normal;                                            // 0x28(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector ImpactNormal;                                      // 0x34(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector TraceStart;                                        // 0x40(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector TraceEnd;                                          // 0x4C(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	float PenetrationDepth;                                  // 0x58(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 Item;                                              // 0x5C(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FName BoneName;                                          // 0x78(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FName MyBoneName;                                        // 0x80(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

enum EFortItemTier : char
{
	No_Tier = 0,
	I = 1,
	II = 2,
	III = 3,
	IV = 4,
	V = 5,
	VI = 6,
	VII = 7,
	VIII = 8,
	IX = 9,
	X = 10,
	NumItemTierValues = 11,
	EFortItemTier_MAX = 12
};

enum class ECollisionChannel : uint8_t
{
	ECC_WorldStatic = 0,
	ECC_WorldDynamic = 1,
	ECC_Pawn = 2,
	ECC_Visibility = 3,
	ECC_Camera = 4,
	ECC_PhysicsBody = 5,
	ECC_Vehicle = 6,
	ECC_Destructible = 7,
	ECC_EngineTraceChannel1 = 8,
	ECC_EngineTraceChannel2 = 9,
	ECC_EngineTraceChannel3 = 10,
	ECC_EngineTraceChannel4 = 11,
	ECC_EngineTraceChannel5 = 12,
	ECC_EngineTraceChannel6 = 13,
	ECC_GameTraceChannel1 = 14,
	ECC_GameTraceChannel2 = 15,
	ECC_GameTraceChannel3 = 16,
	ECC_GameTraceChannel4 = 17,
	ECC_GameTraceChannel5 = 18,
	ECC_GameTraceChannel6 = 19,
	ECC_GameTraceChannel7 = 20,
	ECC_GameTraceChannel8 = 21,
	ECC_GameTraceChannel9 = 22,
	ECC_GameTraceChannel10 = 23,
	ECC_GameTraceChannel11 = 24,
	ECC_GameTraceChannel12 = 25,
	ECC_GameTraceChannel13 = 26,
	ECC_GameTraceChannel14 = 27,
	ECC_GameTraceChannel15 = 28,
	ECC_GameTraceChannel16 = 29,
	ECC_GameTraceChannel17 = 30,
	ECC_GameTraceChannel18 = 31,
	ECC_OverlapAll_Deprecated = 32,
	ECC_MAX = 33
};

enum EMovementMode : uint8
{
	MOVE_None = 0,
	MOVE_Walking = 1,
	MOVE_NavWalking = 2,
	MOVE_Falling = 3,
	MOVE_Swimming = 4,
	MOVE_Flying = 5,
	MOVE_Custom = 6,
	MOVE_MAX = 7,
};

enum ETraceTypeQuery : uint8
{
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	ETraceTypeQuery_MAX = 33,
};

enum EDrawDebugTrace : uint8
{
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4,
};

struct FPawnHighlight
{
	FPawnHighlight() : Priority(), Inner(), Outer(), FresnelBrightness(), FresnelExponent(), UsesPulse() { }
	FPawnHighlight(float Priority, FLinearColor Inner, FLinearColor Outer, float FresnelBrightness, float FresnelExponent, float UsesPulse) : Priority(Priority), Inner(Inner), Outer(Outer), FresnelBrightness(FresnelBrightness), FresnelExponent(FresnelExponent), UsesPulse(UsesPulse) { }

	float Priority;
	FLinearColor Inner;
	FLinearColor Outer;
	float FresnelBrightness;
	float FresnelExponent;
	float UsesPulse;
};

enum EInputEvent : uint8
{
	IE_Pressed = 0,
	IE_Released = 1,
	IE_Repeat = 2,
	IE_DoubleClick = 3,
	IE_Axis = 4,
	IE_MAX = 5
};

struct FGuid {
	int32_t A; // 0x00(0x04)
	int32_t B; // 0x04(0x04)
	int32_t C; // 0x08(0x04)
	int32_t D; // 0x0c(0x04)
};

// Enum Engine.EMaterialParameterAssociation
enum EMaterialParameterAssociation : uint8 {
	LayerParameter = 0,
	BlendParameter = 1,
	GlobalParameter = 2,
	EMaterialParameterAssociation_MAX = 3
};

struct FMaterialParameterInfo {
	FName Name; // 0x00(0x04)

	EMaterialParameterAssociation Association()
	{
		return *(EMaterialParameterAssociation*)(this + 0x04);
	}

	int32_t Index()
	{
		return *(int32_t*)(this + 0x08);
	}
};

struct FVectorParameterValue {
	FMaterialParameterInfo ParameterInfo; // 0x00(0x0c)

	FLinearColor ParameterValue()
	{
		return *(FLinearColor*)(this + 0x0c);
	}

	FGuid ExpressionGUID()
	{
		return *(FGuid*)(this + 0x1c);
	}
};

struct FScalarParameterValue {
	FMaterialParameterInfo ParameterInfo; // 0x00(0x0c)

	float ParameterValue()
	{
		return *(float*)(this + 0x0c);
	}

	FGuid ExpressionGUID()
	{
		return *(FGuid*)(this + 0x10);
	}
};