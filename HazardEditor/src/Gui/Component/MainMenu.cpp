#pragma once

#include "MainMenu.h"
#include "imgui.h"
#include "Gui/Widgets/All.h"


void MainMenu::OnRender()
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("File")) {

		if (ImGui::MenuItem("New")) {
			//Debug::Warn("File->New TODO");
		}

		if (ImGui::MenuItem("Quit")) {
			Hazard::Application::GetCurrent().CloseApplication();
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
			ImGuiLayer::OpenLayer("Inspector");
		}
		if (ImGui::MenuItem("Performance")) {
			ImGuiLayer::OpenLayer("Performance");
		}
		if (ImGui::MenuItem("Viewport")) {
			ImGuiLayer::OpenLayer("Viewport");
		}
		if (ImGui::MenuItem("Console")) {
			ImGuiLayer::OpenLayer("Console");
		}
		if (ImGui::MenuItem("Hierarchy")) {
			ImGuiLayer::OpenLayer("Hierarchy");
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help")) {

		//Debug::Error("Menu Help TODO");

		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}

