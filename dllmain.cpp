#include <includes.h>

std::future <void> main_future;

void DllStart()
{
    GObjects = GetObjects();
    printf("GObjects!\n");

    KismetMathLibrary = UObject::FindObject(L"Engine.Default__KismetMathLibrary");
    GameplayStatics = UObject::FindObject(L"Engine.Default__GameplayStatics");
    KismetMaterialLibrary = UObject::FindObject(L"Engine.Default__KismetMaterialLibrary");
    FortKismetLibrary = UObject::FindObject(L"FortniteGame.Default__FortKismetLibrary");
    KismetSystemLibrary = UObject::FindObject(L"Engine.Default__KismetSystemLibrary");
    KismetStringLibrary = UObject::FindObject(L"Engine.Default__KismetStringLibrary");
    printf("Libraries!\n");

    Keys::LeftMouseButton = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"LeftMouseButton") }, 0 };
    Keys::RightMouseButton = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"RightMouseButton") }, 0 };
    Keys::Insert = FKey{ FName{UKismetStringLibrary::Conv_StringToName(L"Insert") }, 0 };
    Keys::F2 = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"F2") }, 0 };
    Keys::F8 = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"F8") }, 0 };
    Keys::W = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"W") }, 0 };
    Keys::A = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"A") }, 0 };
    Keys::S = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"S") }, 0 };
    Keys::D = FKey{ FName{UKismetStringLibrary::Conv_StringToName(L"D") }, 0 };
    Keys::SpaceBar = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"SpaceBar") }, 0 };
    Keys::LeftShift = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(L"LeftShift") }, 0 };

    Variables::MaterialParameter = UKismetStringLibrary::Conv_StringToName(L"HairTone_Highlights");
    Variables::MaterialParameter2 = UKismetStringLibrary::Conv_StringToName(L"Skin Boost Color And Exponent");
    Variables::MaterialParameter4 = UKismetStringLibrary::Conv_StringToName(L"Primary_Color");
    Variables::MaterialParameter5 = UKismetStringLibrary::Conv_StringToName(L"Emissive_Color");

    UWorld* GWorld = UWorld::GetWorld();
    printf("GWorld: %p\n", GWorld);
    UGameInstance* OwningGameInstance = GWorld->OwningGameInstance();
    ULocalPlayer* LocalPlayer = OwningGameInstance->LocalPlayers()[0];
    UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient();
    printf("ViewportClient: %p\n", ViewportClient);

    // CanvasObject -> temporary canvas if there isn't already one
    //Variables::Canvas = reinterpret_cast<UCanvas*>(UObject::FindObject(L"DebugCanvasObject", reinterpret_cast<UObject*>(-1)));
    UEngine* GEngine = reinterpret_cast<UEngine*>(OwningGameInstance->GetOuterObject());
    MediumFont = GEngine->MediumFont();

    static Hook::NewHook ViewportHook;

    if (ViewportHook.Initialize(ViewportClient))
    {
        ViewportHook.Insert(&DrawTitleSafeArea, Offsets::DrawTitleSafeArea, &DrawTitleSafeAreaOriginal);

        ViewportHook.SwapContext();
    }
}

BOOL DllMain(void* hModule, DWORD ul_reason_for_call, void* lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);
        printf("Injected!\n");
        Sleep(500);

        Variables::BuildTime = GetBuildTime();

        auto EncryptedModule_FortniteClientWin64Shipping = Encrypt(L"FortniteClient-Win64-Shipping.exe");
        GameBase = FindModuleByName(EncryptedModule_FortniteClientWin64Shipping.decrypt()); EncryptedModule_FortniteClientWin64Shipping.clear();
        if (!GameBase) return false;
        printf("GameBase: %p\n", GameBase);

        SpoofCall::Initialize(reinterpret_cast<unsigned char*>(GameBase));
        printf("Initialized SpoofCall\n");

        reinterpret_cast<void(__cdecl*)(uintptr_t, void*)>(DoSpoofCall)(SpoofCode, &DllStart);
    }

    return true;
}