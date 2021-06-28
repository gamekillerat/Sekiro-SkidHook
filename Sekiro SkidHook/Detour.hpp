#pragma once

#include "pch.h"

namespace SkidBase::Hooking
{

	class Detour
	{

	public:

		explicit Detour() {}

		Detour(void * Target, void * Detour, std::string_view Name) :
			m_Target(Target), m_Detour(Detour), m_Name(Name)
		{
		
			auto Result = MH_CreateHook(m_Target, m_Detour, &m_TrampolineAddr);

			if (Result != MH_OK)
				g_Logger->LogError("Cant create hook for {} (error code: {})", m_Name, MH_StatusToString(Result));
#ifdef DEBUG
			else
				g_Logger->LogHook("Hook {} created (addr: 0x{:X})", m_Name, std::uintptr_t(m_Target));
#endif 

			//hde64s HDE = hde64s();

			//m_TrampolineSize = GetTrampolineSize(HDE);	

			//m_TrampolineAddr = CreateTrampoline(m_TrampolineSize, HDE);

		}

	public:

		std::size_t GetTrampolineSize(hde64s HDE)
		{
			std::size_t TrampolineSize = 0;

			if (*std::add_pointer_t<std::uint8_t>(m_Target) == 0xE9)
			{
				m_Target = LPVOID(std::uintptr_t(m_Target) + 5);
			}

			do
			{
				auto AddrToCheck = std::uintptr_t(m_Target) + TrampolineSize;

				TrampolineSize += hde64_disasm(LPVOID(AddrToCheck), &HDE);
			} while (TrampolineSize < 5);

			g_Logger->LogHook("Trampoline for {} hook is {} bytes long", m_Name, TrampolineSize);

			return TrampolineSize;
		}

		void Enable()
		{

			auto Result = MH_EnableHook(m_Target);

			if (Result != MH_OK)
				g_Logger->LogError("Cant enable hook for {} (error code: {})", m_Name, MH_StatusToString(Result));
#ifdef DEBUG
			else
				g_Logger->LogHook("Hooked {} (trampoline addr: 0x{:X})", m_Name, std::uintptr_t(m_TrampolineAddr));
#endif 
			/*DWORD OldProtect = 0;
			VirtualProtect(m_Target, m_TrampolineSize, PAGE_EXECUTE_READWRITE, &OldProtect);

			auto TrampolineRelAddr = std::uintptr_t(m_Detour) - (std::uintptr_t(m_Target) + 5);

			g_Logger->LogHook("Appliying page protection flags on {} to hook", m_Name);

			// just to make sure we dont displace nothing 
			for (int i = 0; i < 5 - 5; i++)
			{
				*std::add_pointer_t<std::uintptr_t>(std::uintptr_t(m_Target) + i) = 0x90;
			}

			*std::add_pointer_t<std::uint8_t>(std::uintptr_t(m_Target)) = 0xE9;
			*std::add_pointer_t<std::uintptr_t>(std::uintptr_t(m_Target) + 1) = TrampolineRelAddr;

			g_Logger->LogHook("Created hook for {} (addr 0x{:X})", m_Name, std::uintptr_t(m_Target));

			if (m_TrampolineSize > 5) // make sure we dont break insrtuctions
			{
				for (int i = 0; i < m_TrampolineSize - 5; i++)
				{
					*std::add_pointer_t<std::uintptr_t>(std::uintptr_t(m_Target) + 5 + i) = 0x90;
				}
			}

			FlushInstructionCache(GetCurrentProcess(), m_Target, m_TrampolineSize);
			VirtualProtect(m_Target, m_TrampolineSize, OldProtect, &OldProtect);*/

		}

		void Disable()
		{

			//memcpy(m_Target, m_TrampolineAddr, m_TrampolineSize);

			//VirtualFree(m_TrampolineAddr, m_TrampolineSize + 5, MEM_RELEASE);
			MH_DisableHook(m_Target);
			MH_RemoveHook(m_Target);

		}

	public:

		std::uintptr_t GetCodeCave(std::size_t TrampolineSize)
		{

			std::uintptr_t StartingAddr = std::uintptr_t(m_Target);

			std::uintptr_t MaxAddr = StartingAddr + 2048; /*2gb*/

			SYSTEM_INFO SystemInfo = SYSTEM_INFO();

			GetSystemInfo(&SystemInfo);

			StartingAddr -= StartingAddr % SystemInfo.dwAllocationGranularity;
			StartingAddr -= SystemInfo.dwAllocationGranularity;

			int Tries = 0;

#ifdef DEBUG
			g_Logger->LogHook("Searching for a code cave for {} hook", m_Name);
#endif
			while (StartingAddr < MaxAddr)
			{

				MEMORY_BASIC_INFORMATION MBI;
				VirtualQuery(LPVOID(StartingAddr), &MBI, sizeof(MBI));

				if (MBI.State == MEM_FREE)
					break;

				if ((ULONG_PTR)MBI.AllocationBase < SystemInfo.dwAllocationGranularity)
					break;

				StartingAddr = (ULONG_PTR)MBI.AllocationBase - SystemInfo.dwAllocationGranularity;

				Tries++;
			}

#ifdef DEBUG
			g_Logger->LogHook("After {} tries, code cave found (addr 0x{:X})", Tries, StartingAddr);
#endif

			return StartingAddr;
		}

		void * CreateTrampoline(std::size_t TrampolineSize, hde64s HDE)
		{

			auto CodeCave = GetCodeCave(TrampolineSize + 5);

			auto TrampolineAddr = VirtualAlloc(LPVOID(CodeCave), TrampolineSize + 5, 
				MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			if (TrampolineAddr != nullptr)
			{
				memcpy(TrampolineAddr, m_Target, TrampolineSize);

				*std::add_pointer_t<std::uint8_t>(std::uintptr_t(TrampolineAddr) + TrampolineSize) = 0xE9;

				auto JumpBackRelAddr = std::uint32_t(std::uintptr_t(m_Target) - (std::uintptr_t(TrampolineAddr) + 5));

				*std::add_pointer_t<std::uintptr_t>(std::uintptr_t(TrampolineAddr) + TrampolineSize + 1) = JumpBackRelAddr;

				g_Logger->LogHook("Created trampoline for {} (addr 0x{:X})", m_Name, std::uintptr_t(TrampolineAddr));

				return TrampolineAddr;
			}

			return nullptr;
		}

		void ReHook()
		{/*this just to be sure*/
			this->Disable();
			this->Enable();
		}

		template<typename T>
		T Original()
		{
			return static_cast<T>(m_TrampolineAddr);
		}

	private:

		void * m_Target;
		void * m_Detour;
		
		void      * m_TrampolineAddr;
		std::size_t m_TrampolineSize;

		std::string_view m_Name;
	};

}