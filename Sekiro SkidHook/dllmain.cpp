
// dllmain.cpp : Define el punto de entrada de la aplicación DLL.
#include "pch.h"

auto MainThread(LPVOID hMod) -> DWORD
{

	SkidBase::g_Logger = std::make_unique<SkidBase::Console::ConsoleMgr>();

	SkidBase::g_Logger->Attach();

	SkidBase::g_Logger->LogInfo("{} Loaded in memory (BaseAddr : 0x{:X})", MENU_NAME, std::uintptr_t(hMod));

	while (FindWindow(nullptr, "Sekiro") == NULL)
	{
		std::this_thread::sleep_for(10ms);
	}

	SkidBase::g_Logger->LogInfo("Initializing pattern scanner...");
	SkidBase::Memory::PatternScanner::InitPatternScanner();

	SkidBase::Pointers::GetPointers();

	SkidBase::g_Logger->LogInfo("Hooking...");
	SkidBase::Hooking::EnableHooks();

	while (!GetAsyncKeyState(VK_INSERT) & 0x1)
	{
		std::this_thread::sleep_for(10ms);
	}

	SkidBase::g_Logger->LogInfo("Unloading {}", MENU_NAME);

	SkidBase::g_Logger->LogInfo("Un-Hooking...");
	SkidBase::Hooking::DisableHooks();

	SkidBase::g_Logger->LogInfo("Disabling rederer...");
	SkidBase::Renderer::UninitializeRenderer();

	SkidBase::g_Logger->Detach();

	FreeLibraryAndExitThread(static_cast<HMODULE>(hMod), TRUE);

	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: { CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr); } break;
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

