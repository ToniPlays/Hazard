#pragma once

#pragma once
#include <hzreditor.h>
#include "Viewport.h"

Viewport::Viewport() : Layer("Viewport")
{

}

bool Viewport::OnEnabled()
{
	renderer = Hazard::ModuleHandler::GetModule<Hazard::RenderEngine>();
	if (renderer != nullptr) return true;
	return false;
}

void Viewport::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;

	static char* data[] = { "Shaded", "Points", "Wireframe" };
	int selected = renderer->GetAPI().GetTypeAsInt();

	ImGui::SetNextItemWidth(125);
	ImGui::Combo("Shading", &selected, data, IM_ARRAYSIZE(data));
	renderer->GetAPI().SetType(selected);


	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::SameLine(0.0f, 30.0f);
	std::stringstream ss;
	ss << size.x << "x" << size.y;
	ImGui::Text(ss.str().c_str());

	ImGui::Image((void*)renderer->GetRenderTexture()->GetColorID(), 
		size, ImVec2(0, 1), ImVec2(1, 0));

	if (size.x != width || size.y != height) {
		
		width = size.x;
		height = size.y;

		renderer->OnViewResized(width, height);
	}
	
	Panel::End();
}


