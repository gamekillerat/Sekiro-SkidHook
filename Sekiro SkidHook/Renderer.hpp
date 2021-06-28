#pragma once
#include "pch.h"

namespace SkidBase::Renderer
{

	inline void CreateRenderTargetView(IDXGISwapChain * SwapChain, ID3D11Device * Device)
	{

		ID3D11Texture2D * BackBuffer = nullptr;
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));

		if (BackBuffer == nullptr)
		{
			g_Logger->LogError("Cant retrieve backbuffer");
			return;
		}

		Device->CreateRenderTargetView(BackBuffer, nullptr, &Pointers::g_RenderTarget);
		BackBuffer->Release();

		g_Logger->LogInfo("Created render target (addr 0x{:X})", std::uintptr_t(Pointers::g_RenderTarget));

		return;
	}

	/* this will fix alpha shit */
	inline void CreateBlend(ID3D11Device * Device)
	{
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(D3D11_BLEND_DESC));

		/*create blend for alpha fixing!!*/
		BlendDesc.RenderTarget[0].BlendEnable = true;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		Device->CreateBlendState(&BlendDesc, &Pointers::g_BlendState);

		if (Pointers::g_BlendState == nullptr)
		{
			g_Logger->LogError("Cant create blend state!");
		}

		g_Logger->LogInfo("Created blend state (addr 0x{:X})", std::uintptr_t(Pointers::g_BlendState));
	}

	inline void NewFrame()
	{
		float blend[4] = { 0 };
		Pointers::g_D3DDeviceContext->OMSetBlendState(Pointers::g_BlendState, blend, 0xffffffff);

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	}
	inline void EndFrame()
	{
		ImGui::Render();

		/*this will make imgui render */
		Pointers::g_D3DDeviceContext->OMSetRenderTargets(1, &Pointers::g_RenderTarget, nullptr);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		//FIX ALPHA 
		float blend[4] = { 0 };
		Pointers::g_D3DDeviceContext->OMSetBlendState(Pointers::g_BlendState, blend, 0xffffffff); //fix alpha
	}

	inline bool InitializeRenderer(IDXGISwapChain * SwapChain)
	{
		static bool Initialized = false;

		if (Initialized == false)
		{
			Pointers::g_DXGISwapChain = Pointers::g_DXGISwapChain;

			Pointers::g_DXGISwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&Pointers::g_D3DDevice));
			Pointers::g_D3DDevice->GetImmediateContext(&Pointers::g_D3DDeviceContext);

			CreateRenderTargetView(SwapChain, Pointers::g_D3DDevice);
			CreateBlend(Pointers::g_D3DDevice);

			ImGui::CreateContext();
			ImGui::StyleColorsLight();

			if (!ImGui_ImplWin32_Init(Pointers::g_GameWindow))
				g_Logger->LogError("Cant initialized ImGui_32");

			if (!ImGui_ImplDX11_Init(Pointers::g_D3DDevice, Pointers::g_D3DDeviceContext))
				g_Logger->LogError("Cant initialized ImGui_D11");

#ifdef DEBUG
			g_Logger->LogInfo("SwapChain found (addr 0x{:X}) || Device found (addr: 0x{:X}) || DeviceCtx found (addr: 0x{:X})",
				std::uintptr_t(Pointers::g_DXGISwapChain), std::uintptr_t(Pointers::g_D3DDevice), std::uintptr_t(Pointers::g_D3DDeviceContext));

			g_Logger->LogScript("Initialized ImGui");

#endif // DEBUG

			Initialized = true;
		}
		return Initialized;
	}
	inline void UninitializeRenderer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImGui::DestroyContext();

		g_Logger->LogScript("Uninitialized ImGui");

		Pointers::g_BlendState->Release();
		Pointers::g_RenderTarget->Release();
	}
}