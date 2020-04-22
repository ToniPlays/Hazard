#pragma once
#include "HzrEditor.h"
#include "Performance.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "EditorGui/Editor/EditorGUI.h"

bool Performance::layerOpen = true;

void Performance::OnRender() const
{
	if (!layerOpen) return;
	ImGui::Begin("Perfomance", &layerOpen);

	if (ImGui::Button("Change clear color")) {
		EditorGUI::OpenPicker([](Hazard::Color color) {
			Hazard::Application::Get().GetWindow().SetClearColor(color);
			}, Hazard::Application::Get().GetWindow().GetClearColor());
	}

	ImGui::SameLine(175);
	static bool vsync = false;
	ImGui::Checkbox("VSync", &vsync);

	Hazard::Application::Get().GetWindow().SetVSync(vsync);

	std::stringstream ss;
	ss << "Frametime " << Hazard::Time::unscaledDeltaTime * 1000 << " ms";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine(200);

	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "FPS " << 1 / Hazard::Time::unscaledDeltaTime;
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "Renderer: " << glGetString(GL_RENDERER);
	ImGui::Text(ss.str().c_str());

	ImGui::End();
}
