// pch.h: este es un archivo de encabezado precompilado.
// Los archivos que se muestran a continuación se compilan solo una vez, lo que mejora el rendimiento de la compilación en futuras compilaciones.
// Esto también afecta al rendimiento de IntelliSense, incluida la integridad del código y muchas funciones de exploración del código.
// Sin embargo, los archivos que se muestran aquí se vuelven TODOS a compilar si alguno de ellos se actualiza entre compilaciones.
// No agregue aquí los archivos que se vayan a actualizar con frecuencia, ya que esto invalida la ventaja de rendimiento.

#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "fmt.lib")
#include <fmt/format.h>

#pragma comment (lib, "MinHook-Release.lib")
#include <minhook/MinHook.h>

#include <hde/hde64.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment (lib, "d3d11.lib")
#include <d3d11.h>

#pragma comment (lib, "dxgi.lib")
#include <dxgi.h>

#pragma comment (lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include <set>
#include <map>
#include <ShlObj_core.h>
#include <iostream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <fstream>
#include <vector>

#define MENU_NAME "SkidBase"
#define DEBUG

using namespace std::chrono_literals;

#include "Typedefs.hpp"

#include "Console.hpp"

namespace SkidBase
{
	inline std::unique_ptr<Console::ConsoleMgr> g_Logger;
}

#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT Msg, WPARAM Key, LPARAM KeyInfo);

#include "ModuleMgr.hpp"

#include "PointerConversion.hpp"
#include "PatternScanner.hpp"

#include "Pointers.h"

#include "Detour.hpp"
#include "VMT.hpp"

#include "Renderer.hpp"

#include "MainMenu.h"

#include "HookDecls.h"

#endif //PCH_H
