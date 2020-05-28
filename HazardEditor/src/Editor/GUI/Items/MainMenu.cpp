#pragma once

#include "MainMenu.h"
#include "imgui.h"

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
			
		}
		if (ImGui::MenuItem("Performance")) {
			
		}
		if (ImGui::MenuItem("Viewport")) {
		
		}
		if (ImGui::MenuItem("Console")) {
			
		}
		if (ImGui::MenuItem("Hierarchy")) {
			
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

