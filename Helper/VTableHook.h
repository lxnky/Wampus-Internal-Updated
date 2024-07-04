#pragma once

namespace Hook
{
	class NewHook {
	private:

		uintptr_t Address;
		uintptr_t* OriginalVTable;
		uintptr_t* AllocatedVTable;
		int Size;

		uintptr_t LastHookedFunctionAddress;
		int LastHookedFunctionIndex;

	public:

		NewHook() { } //It has to be static!!

		static bool ValidPointer(uintptr_t Pointer)
		{
			return Pointer && (Pointer >= 0x10000) && (Pointer < 0xF000000000000);
		}

		bool Initialize(void* Address)
		{
			if (!Address) return false;

			this->Address = uintptr_t(Address);
			this->OriginalVTable = *(uintptr_t**)(Address);

			if (this->LastHookedFunctionAddress && this->LastHookedFunctionIndex) //Not first time hooking
			{
				if (this->LastHookedFunctionAddress == this->OriginalVTable[this->LastHookedFunctionIndex]) //Already hooked
				{
					return false;
				}
			}

			this->Size = 0;

			while (NewHook::ValidPointer(this->OriginalVTable[this->Size]))
			{
				this->Size += 1;
			}

			this->AllocatedVTable = reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t(__cdecl*)(uintptr_t, int, uintptr_t, void*)>(DoSpoofCall)(this->Size * 8, 0x10, SpoofCode, (void*)(GameBase + Offsets::Malloc)));

			for (int i = 0; i < this->Size; i++)
			{
				this->AllocatedVTable[i] = this->OriginalVTable[i];
			}

			return true;
		}

		template <typename OriginalType>
		void Insert(void* HookFunction, int Index, OriginalType* OutOriginalFunction)
		{
			if (Index < this->Size) //Check if index is even inside the VTable
			{
				void* OriginalFunction = (void*)this->OriginalVTable[Index];
				if (OriginalFunction != HookFunction) //Function isn't already contained in fake VTable
				{
					*OutOriginalFunction = OriginalType(OriginalFunction);

					this->AllocatedVTable[Index] = uintptr_t(HookFunction);

					this->LastHookedFunctionAddress = uintptr_t(HookFunction);
					this->LastHookedFunctionIndex = Index;
				}
			}
		}

		void SwapContext()
		{
			*(uintptr_t**)(this->Address) = this->AllocatedVTable;
		}
	};
}