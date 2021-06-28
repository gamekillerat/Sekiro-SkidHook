#include "pch.h"

namespace SkidBase::Hooking
{

	HRESULT PresentHook(IDXGISwapChain * SwapChain, UINT Flag, UINT VSync)
	{

		static bool Init = false;

		if (Init == false)
		{
			Pointers::g_DXGISwapChain = SwapChain;

			Init = true;
			return S_OK;
		}

		if (SwapChain == Pointers::g_DXGISwapChain)
		{
			if (Renderer::InitializeRenderer(SwapChain))
			{
				Renderer::NewFrame();
				MainMenu::DrawMainMenu();
				Renderer::EndFrame();
			}
		}

		return g_PresentHK.Original<decltype(&PresentHook)>()(SwapChain, Flag, VSync);
	}

	HRESULT D3D11CreateDeviceHook(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software,
		UINT Flags, D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
		ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext)
	{

		auto Result = g_CreateDeviceHK.Original<decltype(&D3D11CreateDeviceHook)>()(pAdapter, DriverType, Software,
			Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);

		static bool IsPresentHooked = false;

		if (IsPresentHooked == false 
			&& ppDevice != nullptr 
			&& ppImmediateContext != nullptr)
		{
			g_Logger->LogScript("D3D11CreateDevice hook called, retrieving a working ID3D11DeviceContext* & ID3D11Device*...");

			Pointers::g_D3DDevice = *ppDevice;
			Pointers::g_D3DDeviceContext = *ppImmediateContext;

			IsPresentHooked = true;

		}

		return Result;
	}

	LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM Key, LPARAM KeyInfo)
	{

		ImGui_ImplWin32_WndProcHandler(hWnd, Msg, Key, KeyInfo);

		return DefWindowProc(hWnd, Msg, Key, KeyInfo);
	}

}