#pragma once

#include "pch.h"

namespace SkidBase::Pointers
{

	inline HWND g_GameWindow = nullptr;

	inline IDXGISwapChain      * g_DXGISwapChain = nullptr;
	inline ID3D11DeviceContext * g_D3DDeviceContext = nullptr;
	inline ID3D11Device        * g_D3DDevice = nullptr;

	inline ID3D11RenderTargetView * g_RenderTarget = nullptr;
	inline ID3D11BlendState       * g_BlendState = nullptr; /*fix alpha*/

	inline void GetPointers()
	{

		/* PS = PatternScanner omfg skid shut up nigga*/
		namespace PS = Memory::PatternScanner;


		g_GameWindow = FindWindow(nullptr, "Sekiro");
	
		PS::FindPatterns();
	}


}