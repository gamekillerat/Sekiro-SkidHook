#pragma once
#include "pch.h"

namespace SkidBase::Memory::PatternScanner
{

	struct PatternDecl
	{
		std::string_view Pattern;
		
		std::string_view PatternName;

		std::function<void(void *)> Function;

		explicit PatternDecl(std::string_view Sig, std::string_view Name, std::function<void(void *)> Func) :
			Pattern(Sig), PatternName(Name), Function(Func) 
		{}

	}; inline std::vector<PatternDecl> g_Patterns;

	inline std::unique_ptr<Memory::ModuleInfo> g_ModuleInfo;

	inline void InitPatternScanner()
	{

		g_ModuleInfo = Memory::GetModuleInfo(NULL);

		g_Logger->LogInfo("Sekiro Module Info -> ProcessId {} \n " \
		"Process BaseAddr: 0x{:X} (Size 0x{:X}) || {} BaseAddr 0x{:X}",
		GetCurrentProcessId(), g_ModuleInfo->StartAddr, g_ModuleInfo->SizeOfMod, 
		MENU_NAME, std::uintptr_t(GetModuleHandle("Sekiro SkidHook.dll")));

	}

	inline void AddPattern(PatternDecl Pattern)
	{
		g_Patterns.push_back(Pattern);
	}

	inline Memory::Ptr GetPatternPointer(const char* Pattern, const char* Name)
	{
		auto PointerRetrieved = Memory::Ptr(0);

		//Lambda with bool return lul
		auto CompareMemoryWithPattern = [](std::uint8_t* ByteToSearchIn, std::vector<std::uint8_t> BytesArray) -> bool {

			//iterate over the pattern
			for (int i = 0; i < BytesArray.size(); i++) {

				/*check if the current byte is a wildcard to skip lul*/
				if (BytesArray[i] == 0x00) continue;

				if (ByteToSearchIn[i] != BytesArray[i])
					return false;
				//check if any of the bytes of the pattern match with the data given

				//if the given data doesnt match with a single byte of the pattern it will return false
			}

			return true;
		};

		/*transform the pattern onto raw hex bytes*/
		std::vector<std::uint8_t> PatternBytes;

		for (int i = 0; i < strlen(Pattern); i++) {

			if (Pattern[i] == ' ') continue;

			/*if theres a wildcard we actually dont care of the byte*/
			if (Pattern[i] == '?') {

				PatternBytes.push_back(0x00);
				continue;
			}

			char ByteArray[3] = { Pattern[i], Pattern[i + 1], '\0' };

			/*get decimal number of the bytes allocated on the string, and convert them from hex with a 16 radix*/
			auto BytesConverted = strtol(ByteArray, nullptr, 16);
			PatternBytes.push_back(static_cast<std::uint8_t>(BytesConverted));

			i++;
		}

		for (auto i = g_ModuleInfo->StartAddr; i != g_ModuleInfo->EndAddr; i++) {

			if (CompareMemoryWithPattern(reinterpret_cast<std::uint8_t*>(i), PatternBytes)) {

				PointerRetrieved = Memory::Ptr(i);
				break;
			}

		}

		// clear the array of the pattern bytes
		PatternBytes.clear();
		return PointerRetrieved;
	}

	inline void FindPatterns()
	{
		int PatternsFound = 0;
		for (auto Pattern : g_Patterns)
		{

			auto PatternPtr = GetPatternPointer(Pattern.Pattern.data(), Pattern.PatternName.data());

			if (PatternPtr.Get() == 0)
			{
				g_Logger->LogError("Cant retrieve {} pattern pointer", Pattern.PatternName);
				continue;
			}

#ifdef DEBUG
			g_Logger->LogPattern("Found {} pattern (addr 0x{:X})", Pattern.PatternName, PatternPtr.Get());
#endif 
			std::invoke(Pattern.Function, PatternPtr.As<void*>());

			PatternsFound++;
		}

		g_Logger->LogPattern("Found [{} / {}] patterns", PatternsFound, g_Patterns.size());
	}

}