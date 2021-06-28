#pragma once
#include "pch.h"

namespace SkidBase::Hooking
{

	inline auto g_PresentHK = Detour();
	extern HRESULT PresentHook(IDXGISwapChain * SwapChain, UINT Flag, UINT VSync);


	inline auto g_CreateDeviceHK = Detour();
	extern HRESULT D3D11CreateDeviceHook(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
		UINT Flags, D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
		ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext);


	inline void EnableHooks()
	{
		MH_Initialize();

		/*auto D3D11CreateDeviceAddr = GetProcAddress(GetModuleHandle("d3d11.dll"), "D3D11CreateDevice");

		if (D3D11CreateDeviceAddr != nullptr)
		{
			g_CreateDeviceHK = Detour(D3D11CreateDeviceAddr,
				&D3D11CreateDeviceHook, "D3D11CreateDevice");
			g_CreateDeviceHK.Enable();
		}
		else g_Logger->LogError("Cant retrieve D3D11CreateDevice(d3d11.dll) address");*/

		auto PresentAddr = std::uintptr_t(GetModuleHandle("dxgi.dll")) + 0x4670;

		g_PresentHK = Detour(LPVOID(PresentAddr), &PresentHook, "Present");
		g_PresentHK.Enable();

	}

	inline void DisableHooks()
	{
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
	
		MH_Uninitialize();
	}

}