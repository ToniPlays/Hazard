#pragma once

#pragma once
#include "Viewport.h"
#include "imgui.h"
#include <sstream>
#include "Editor/Utils/Utility.h"
#include "Editor/GUI/Editor.h"

bool Viewport::layerOpen = true;

Viewport::Viewport()
{

}

bool Viewport::OnEnabled()
{
	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
	if (renderer != nullptr) return true;
	return false;
}

void Viewport::Render()
{
	if (!layerOpen) return;

	ImGui::Begin("Viewport", &layerOpen);

	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();

	static char* data[] = { "Shaded", "Points", "Wireframe" };
	int selected = 0;
	renderer->GetAPI().GetType(selected);

	ImGui::SetNextItemWidth(125);
	ImGui::Combo("Shading", &selected, data, IM_ARRAYSIZE(data));
	renderer->GetAPI().SetType(selected);

	ImGui::SameLine();
	ImGui::Text("Scene color");
	ImGui::SameLine();
	Hazard::Color sceneColor = renderer->GetWindow().GetClearColor();
	if (ImGui::ColorButton("Scene color", ColorAsImVec(sceneColor))) {
		Editor::OpenColorPicker(sceneColor, [](Hazard::Color color) {
			Hazard::GlobalRenderer* rd = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
		});
	}

	ImVec2 size = ImGui::GetWindowSize();
	size.y -= 50;
	renderer->SceneRender();

	ImGui::Image((void*)renderer->GetRenderTexture()->GetColorID(), 
		GetMaxSize(size, renderer->GetRenderTexture()->GetWidth(), renderer->GetRenderTexture()->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}


