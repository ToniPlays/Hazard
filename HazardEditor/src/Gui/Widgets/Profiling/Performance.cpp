#pragma once
#include "HzrEditor.h"
#include "Performance.h"
#include "imgui_internal.h"
#include "Core/ImGuiLayer.h"

bool Performance::layerOpen = true;

void Performance::OnRender() const
{
	if (!layerOpen) return;

	ImGui::Begin("Perfomance", &layerOpen);

	if (ImGui::Button("Change clear color")) {
		ImGuiLayer::OpenPicker([](Hazard::Color color) {
			Hazard::Application::GetCurrent().GetWindow().SetClearColor(color);
			}, Hazard::Application::GetCurrent().GetWindow().GetClearColor());
	}

	ImGui::SameLine(175);
	static bool vsync = false;
	ImGui::Checkbox("VSync", &vsync);

	Hazard::Application::GetCurrent().GetWindow().SetVSync(vsync);

	std::stringstream ss;
	ss << "Frametime " << Hazard::Time::unscaledDeltaTime << "ms";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ImGui::SameLine(200);
	ss << "FPS " << Math::Round(1 / Hazard::Time::unscaledDeltaTime * 1000, 2);
	ImGui::Text(ss.str().c_str());


	ss.str("");
	ss << "Window size: " << Hazard::Application::GetCurrent().GetWindow().GetWidth() << "x" << Hazard::Application::GetCurrent().GetWindow().GetHeight();
	ImGui::Text(ss.str().c_str());
	ss.str("");


	ss.str("");
	ss << "Platform: " << Hazard::Application::GetCurrent().GetWindow().GetWindowInfo().Platform;
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "Renderer: " << Hazard::Application::GetCurrent().GetWindow().GetWindowInfo().Renderer;
	ImGui::Text(ss.str().c_str());

	ImGui::NewLine();

	ImGui::Text("Active modules: ");
	for (Hazard::Module* module : Hazard::Application::GetCurrent().GetModuleHandler().GetAll()) {
		ss.str("");
		ss << "- " + module->GetName();
		ImGui::Text(ss.str().c_str());
	}

	ImGui::End();
}
void Performance::OpenLayer() const
{
	//Debug::Log("Opened Performance");
	layerOpen = true;
}

void Performance::CloseLayer() const
{
	//Debug::Log("Closed Performance");
	layerOpen = false;
}
