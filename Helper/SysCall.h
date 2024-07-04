#pragma once

typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation
} MEMORY_INFORMATION_CLASS;

extern "C" long __query_virtual_memory(uintptr_t ProcessHandle, void* BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, void* MemoryInformation, uintptr_t MemoryInformationLength, uintptr_t * ReturnLenght);