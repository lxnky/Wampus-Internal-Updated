#pragma once

#define FIELD_OFFSET(type, field)    ((long)(long long)&(((type *)0)->field))

#define IMAGE_FIRST_SECTION( ntheader ) ((IMAGE_SECTION_HEADER*)        \
    ((uintptr_t)(ntheader) +                                            \
     FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) +                 \
     ((ntheader))->FileHeader.SizeOfOptionalHeader   \
    ))

extern "C" void DoSpoofCall();

extern "C" uintptr_t CallReturns[];
extern "C" uintptr_t * CallFakestack;
extern "C" uintptr_t CallFakestackSize;

namespace SpoofCall
{
	__forceinline void Initialize(unsigned char* module, unsigned int max_fakestack = 14)
	{
		map<signed char, vector<uintptr_t>> proxy_clean_returns;

		auto dos = reinterpret_cast<IMAGE_DOS_HEADER*>(module);
		auto nt = reinterpret_cast<IMAGE_NT_HEADERS*>(module + dos->e_lfanew);
		auto image_size = nt->OptionalHeader.SizeOfImage;

		auto section = IMAGE_FIRST_SECTION(nt);

		MEMORY_BASIC_INFORMATION mbi;

		for (auto i = 0; i < nt->FileHeader.NumberOfSections; i++)
		{
			if ((section->Characteristics & 0x02000000) == 0) //image_scn_mem_discardable
			{
				auto address = module + section->VirtualAddress;
				while (true)
				{
					__memset(&mbi, 0, sizeof(mbi));

					if (__query_virtual_memory(-1, address, MemoryBasicInformation, &mbi, sizeof(mbi), nullptr) != 0)
						break;

					auto base_page = (unsigned char*)mbi.BaseAddress;

					if (mbi.Protect == 0x20 || //page_execute_read
						mbi.Protect == 0x40 || //page_execute_readwrite
						mbi.Protect == 0x80) //page_execute_writecopy
					{
						for (auto i = 0u; i < (mbi.RegionSize - 0x10); i++)
						{
							//add rsp, xx
							//ret
							if (base_page[i] == 0x48 &&
								base_page[i + 1] == 0x83 &&
								base_page[i + 2] == 0xc4 &&
								base_page[i + 4] == 0xc3)
							{
								proxy_clean_returns[base_page[i + 3]].push_back(uintptr_t(base_page + i));
							}
						}
					}
					address = base_page + mbi.RegionSize;
					if (address >= (module + section->VirtualAddress + section->Misc.VirtualSize))
						break;
				}
			}
			section++;
		}

		vector<signed char> proxy_clean_returns_keys;
		proxy_clean_returns_keys.reserve(proxy_clean_returns.size());

		vector<uintptr_t> fakestack;
		fakestack.reserve(max_fakestack * 2);

		for (auto& it : proxy_clean_returns)
		{
			const auto index = (it.first / sizeof(uintptr_t));
			CallReturns[index] = it.second.at(__rand() % it.second.size());

			if (index < 10 && index % 2 == 1)
				proxy_clean_returns_keys.push_back(it.first);
		}

		while (fakestack.size() < max_fakestack)
		{
			const auto pseudo_random_number = __rand();
			const auto return_length = proxy_clean_returns_keys.at(pseudo_random_number % proxy_clean_returns_keys.size());
			const auto params = (return_length / sizeof(uintptr_t));
			const auto& address_array = proxy_clean_returns[return_length];
			const auto random_address = address_array.at(pseudo_random_number % address_array.size());

			fakestack.push_back(random_address);
			for (auto i = 0u; i < params; i++)
				fakestack.push_back(uintptr_t(module) + (__rand() % image_size));
		}

		CallFakestackSize = fakestack.size();
		CallFakestack = new uintptr_t[fakestack.size()];
		__memcpy(CallFakestack, fakestack.data(), CallFakestackSize * 8);
	}
}