#pragma once

#include "MainMenu.h"
#include "imgui.h"
#include "EditorGui/Editor/EditorGUI.h"


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

