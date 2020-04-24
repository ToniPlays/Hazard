#pragma once
#include "HzrEditor.h"
#include "Performance.h"
#include "imgui_internal.h"
#include "Editor/EditorGUI.h"

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
	ss << "FPS " << 1 / Hazard::Time::unscaledDeltaTime;
	ImGui::Text(ss.str().c_str());


	ss.str("");
	ss << "Window size: " << Hazard::Application::Get().GetWindow().GetWidth() << "x" << Hazard::Application::Get().GetWindow().GetHeight();
	ImGui::Text(ss.str().c_str());
	ss.str("");


	ss.str("");
	ss << "Renderer: " << Hazard::Application::GetInfo().renderer.graphicProcessor;
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "GPU: " << Hazard::Application::GetInfo().renderer.renderer;
	ImGui::Text(ss.str().c_str());


	ImGui::End();
}
void Performance::OpenLayer() const
{
	Debug::Log("Opened Performance");
	layerOpen = true;
}

void Performance::CloseLayer() const
{
	Debug::Log("Closed Performance");
	layerOpen = false;
}
