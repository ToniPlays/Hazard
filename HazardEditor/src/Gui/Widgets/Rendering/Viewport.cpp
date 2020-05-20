#pragma once
#include "imgui.h"
#include "Viewport.h"
#include "glad/glad.h"

bool Viewport::isOpen = true;
Viewport::Viewport()
{
	texture = new Hazard::RenderTexture("Viewport Texture", 1280, 720, Hazard::Color::FromHex("#147300"));
}
void Viewport::OnRender() const
{
	if (!isOpen) return;
	ImGui::Begin("Viewport", &isOpen);

    static int selected = 0;
	static const char* items[] = { "Shaded", "Unlit", "Wireframe", "Albedo" };
	ImGui::SetNextItemWidth(125);
	ImGui::Combo("Shading", &selected, items, IM_ARRAYSIZE(items));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(25);
	
	//Render scene
	Hazard::GlobalRenderer* renderer = Hazard::Application::GetCurrent().GetModuleHandler().GetModule<Hazard::GlobalRenderer>();
	renderer->renderTarget = texture;
	renderer->SceneRender();

	ImGui::SameLine();
	std::stringstream ss;
	ImVec2 windowSize = ImGui::GetWindowSize();

	ss << "Rendering to: " << texture->name << ", size " << texture->GetWidth() << "x" << texture->GetHeight();
	ImGui::Text(ss.str().c_str());

	ImVec2 cursorPos = ImVec2((windowSize.x - texture->GetWidth()) * 0.5f, (windowSize.y - texture->GetHeight()) * 0.5f + 15);
	ImGui::SetCursorPos(cursorPos);

	ImGui::Image((void*)(intptr_t)texture->GetTextureID(), ImVec2(1280, 720), ImVec2(1, 0), ImVec2(0, 1));
	ImGui::End();
}
void Viewport::OpenLayer() const
{
	//Debug::Log("Opened Viewport");
	isOpen = true;
}

void Viewport::CloseLayer() const
{
	//Debug::Log("Closed Viewport");
	isOpen = false;
}


