#pragma once

#pragma once
#include <hzreditor.h>
#include "Viewport.h"

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

	ImGui::SameLine(0.0f, 20.0f);
	ImGui::Text("Scene color");
	ImGui::SameLine();

	Hazard::Color sceneColor = renderer->GetWindow().GetClearColor();

	if (ImGui::ColorButton("Scene color", ColorAsImVec(sceneColor))) {
		Editor::OpenColorPicker(sceneColor, [](Hazard::Color color) {
			Hazard::GlobalRenderer* rd = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
			rd->GetWindow().SetClearColor(color);
			});
	}

	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::SameLine(0.0f, 30.0f);
	std::stringstream ss;
	ss << size.x << "x" << size.y;
	ImGui::Text(ss.str().c_str());

	if (size.x != width || size.y != height) {
		
		width = size.x;
		height = size.y;

		Hazard::WindowResizeEvent e(width, height);
		renderer->OnResized(e);
	}
	renderer->SceneRender();

	ImGui::Image((void*)renderer->GetRenderTexture()->GetColorID(), 
		size, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}


