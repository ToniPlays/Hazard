#pragma once
#include <hzrpch.h>

#include "MainMenu.h"
#include "imgui.h"
#include "Core/Application.h"

namespace Hazard {

	void MainMenu::OnRender() const
	{
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Quit")) {
				Application::Close();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}