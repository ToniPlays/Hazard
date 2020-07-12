#pragma once
#include <hzreditor.h>
#include "Editor/GUI/All.h"
#include "MainMenu.h"

void SplitString(std::string string, const char* regex, char* out) {
	char splitted[3];

	out = splitted;
}

void MainMenu::OnRender()
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("File")) {

		if (ImGui::MenuItem("New")) {
			//Debug::Warn("File->New TODO");
		}

		if (ImGui::MenuItem("Quit")) {
			Hazard::WindowCloseEvent e;
			Hazard::HazardLoop::GetCurrent().OnEvent(e);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {

		//Debug::Error("Menu Edit TODO");

		ImGui::EndMenu();
	}
	//Assetmenu

	if (ImGui::BeginMenu("Assets")) {

		if (ImGui::MenuItem("New GameObject")) {
			//CreateGameObject::CreateEmpty();
		}
		if (ImGui::MenuItem("Import asset")) {
			//Debug::Warn("Assets-> Import asset TODO");
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window")) {
		if (ImGui::MenuItem("Inspector")) {
			Editor::SetLayerOpen<Inspector>(true);
		}
		if (ImGui::MenuItem("Performance")) {
			Editor::SetLayerOpen<Performance>(true);
		}
		if (ImGui::MenuItem("Viewport")) {
			Editor::SetLayerOpen<Viewport>(true);
		}
		if (ImGui::MenuItem("Console")) {
			Editor::SetLayerOpen<Console>(true);
		}
		if (ImGui::MenuItem("Hierarchy")) {
			//Editor::SetLayerOpen<Hierarchy>(true);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help")) {

		//Debug::Error("Menu Help TODO");

		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}

void MainMenu::CreateMenu(std::string menu)
{
	ImGui::MenuItem(menu.c_str());
}

