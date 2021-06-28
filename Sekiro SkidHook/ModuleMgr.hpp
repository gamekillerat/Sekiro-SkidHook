#pragma once

#include "pch.h"

namespace SkidBase::Memory
{

	struct ModuleInfo
	{

		std::uintptr_t StartAddr;
		std::uintptr_t EndAddr;

		std::uintptr_t DosHeaderAddr;
		std::uintptr_t NtHeaderAddr;

		std::uintptr_t SizeOfMod;

		HMODULE        HMod;

		template<typename T>
		auto GetImport(std::string_view ImportName) -> T /* i code like this because im 1337 pro coder 100% legit man*/
		{
			return reinterpret_cast<T>(GetProcAddress(HMod, ImportName.data()));
		}

	};

	inline std::unique_ptr<ModuleInfo> GetModuleInfo(const char * ModuleName)
	{

		auto ModuleHandle = GetModuleHandle(ModuleName);

		auto DosHeaders = std::add_pointer_t<IMAGE_DOS_HEADER>(std::uintptr_t(ModuleHandle));
		auto NtHeaders = std::add_pointer_t<IMAGE_NT_HEADERS>(std::uintptr_t(ModuleHandle) + DosHeaders->e_lfanew);

		auto Size = NtHeaders->OptionalHeader.SizeOfImage;

		ModuleInfo * ModInfo = new ModuleInfo();

		ModInfo->StartAddr = std::uintptr_t(ModuleHandle);
		ModInfo->EndAddr = std::uintptr_t(ModuleHandle) + Size;

		ModInfo->SizeOfMod = Size;

		ModInfo->DosHeaderAddr = std::uintptr_t(DosHeaders);
		ModInfo->NtHeaderAddr = std::uintptr_t(NtHeaders);

		return std::unique_ptr<ModuleInfo>(ModInfo);
	}

}