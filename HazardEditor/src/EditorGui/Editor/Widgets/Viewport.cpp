#pragma once
#include "imgui.h"
#include "Viewport.h"

bool Viewport::isOpen = false;
void Viewport::OnRender() const
{
	if (!isOpen) return;


	ImGui::Begin("Viewport", &isOpen);

	ImGui::End();
}
