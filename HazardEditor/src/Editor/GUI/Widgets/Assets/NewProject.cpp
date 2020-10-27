#pragma once
#include <hzreditor.h>
#include "NewProject.h"
#include "Editor/GUI/GUILibrary/Panel.h"

NewProject::NewProject() : Layer("New project")
{
}

bool NewProject::OnEnabled()
{
	SetLayerOpen(false);
	return true;
}

void NewProject::Render()
{
	if (!isLayerOpen) return;
	
	ImVec2 size =  ImGui::GetWindowSize();
	size.x /= 2;
	size.y /= 3.0f;

	ImGui::SetNextWindowPos(size);
	ImGui::SetNextWindowSize(size);

	ImGui::OpenPopup(name.c_str());
	ImGui::BeginPopupModal(name.c_str(), &isLayerOpen, ImGuiWindowFlags_NoResize);

	ImGui::Columns(2);
	Style::SelectFont(1);
	ImGui::Text("New project");
	ImGui::PopFont();



	ImGui::EndPopup();
}
