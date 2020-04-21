#pragma once
#include <hzrpch.h>
#include "MainMenu.h"
#include "imgui.h"
#include "Core/Application.h"
#include "../EditorGUI.h"

namespace Hazard {
	void MainMenu::OnRender()
	{
		ImGui::BeginMenuBar();

		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Quit")) {
				Hazard::Application::Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem("Performance")) {
				EditorGUI::OpenLayer("Performance");
			}
			if (ImGui::MenuItem("Viewport")) {
				EditorGUI::OpenLayer("Viewport");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
