#pragma once

#pragma once
#include <hzreditor.h>
#include "Viewport.h"

Viewport::Viewport() : Layer("Viewport")
{

}

Viewport::~Viewport()
{

}

bool Viewport::OnEnabled()
{
	renderer = Hazard::ModuleHandler::GetModule<Hazard::RenderEngine>();
	return renderer != nullptr;
}

void Viewport::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;

	
	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)renderer->GetRenderTexture().GetColorID(), 
		size, ImVec2(0, 1), ImVec2(1, 0));

	if (size.x != width || size.y != height) {
		
		width = size.x;
		height = size.y;

		renderer->OnViewResized(width, height);
	}
	
	Panel::End();
}


