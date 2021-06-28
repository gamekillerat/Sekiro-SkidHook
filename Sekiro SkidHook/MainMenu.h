#pragma once

#include "pch.h"

namespace SkidBase::MainMenu
{

	inline void DrawMainMenu()
	{
		if (ImGui::Begin("Main Menu"))
		{
			ImGui::Button("ImGui");
		}
		ImGui::End();
	}


}
