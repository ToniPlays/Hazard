#pragma once
#include "imgui.h"
#include "Viewport.h"

bool Viewport::isOpen = true;
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
	static bool s1 = true;
	ImGui::Checkbox("2D", &s1);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(25);
	static bool s2 = false;
	ImGui::Checkbox("Audio", &s2);
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


