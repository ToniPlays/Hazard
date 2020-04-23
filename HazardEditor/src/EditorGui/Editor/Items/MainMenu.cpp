#pragma once

#include "MainMenu.h"
#include "imgui.h"
#include "EditorGui/Editor/EditorGUI.h"


void MainMenu::OnRender()
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("File")) {

		if (ImGui::MenuItem("New")) {
			Debug::Log("MenuItem in File->New TODO");
		}

		if (ImGui::MenuItem("Quit")) {
			Hazard::Application::Close();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {

		Debug::Log("Menu Edit TODO");

		ImGui::EndMenu();
	}
	//Assetmenu

	if (ImGui::BeginMenu("Assets")) {

		

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window")) {
		if (ImGui::MenuItem("Inspector")) {
			EditorGUI::OpenLayer("Inspector");
		}
		if (ImGui::MenuItem("Performance")) {
			EditorGUI::OpenLayer("Performance");
		}
		if (ImGui::MenuItem("Viewport")) {
			EditorGUI::OpenLayer("Viewport");
		}
		if (ImGui::MenuItem("Console")) {
			EditorGUI::OpenLayer("Console");
		}
		if (ImGui::MenuItem("Hierarchy")) {
			EditorGUI::OpenLayer("Hierarchy");
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help")) {
		Debug::Log("Menu Help TODO");
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}

