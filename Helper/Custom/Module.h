#pragma once

uintptr_t FindModuleByName(wchar_t* ModuleName)
{
    printf("ModuleName: %ls\n", ModuleName);
    PPEB Peb = reinterpret_cast<PPEB>(GetPEB());
    if (!Peb) return 0;

    printf("Process Enviroment Structure: %p\n", Peb);

    if (PPEB_LDR_DATA pLdr = Peb->Ldr)
    {
        for (PLIST_ENTRY ListEntry = (PLIST_ENTRY)pLdr->InMemoryOrderModuleList.Flink;
            ListEntry != &pLdr->InMemoryOrderModuleList;
            ListEntry = (PLIST_ENTRY)ListEntry->Flink) {

            PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            printf("DllName: %ls\n", pEntry->FullDllName.Buffer);
            if (__wcsicmp_insensitive(pEntry->FullDllName.Buffer, ModuleName))
            {
                return uintptr_t(pEntry->DllBase);
            }
        }
    }

    return 0;
}