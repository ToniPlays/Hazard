#pragma once
#include "Performance.h"
#include "imgui.h"
#include <sstream>

bool Performance::layerOpen = true;

Performance::Performance()
{
}


void Performance::Render()
{
	if (!layerOpen) return;

	ImGui::Begin("Performance", &layerOpen);
	
	Hazard::Renderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::Renderer>();
	
	ImGui::Checkbox("Use gradient", &renderer->useGradient);
	if(renderer->useGradient)
		ImGui::SliderFloat("Blue channel", &renderer->test, 0.0f, 1.0f);
	ImGui::NewLine();
	/*
	if (ImGui::Button("Change clear color")) {
		/*ImGuiLayer::OpenPicker([](Hazard::Color color) {
			Hazard::Application::GetCurrent().GetWindow().SetClearColor(color);
			}, Hazard::Application::GetCurrent().GetWindow().GetClearColor());
	}*/
	ImGui::NewLine();
	static bool vsync = false;
	ImGui::Checkbox("VSync", &vsync);

	renderer->GetWindow().SetVSync(vsync);

	std::stringstream ss;
	ss << "Frametime " << Hazard::Time::unscaledDeltaTime << "ms";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "FPS " << Math::Round(1 / Hazard::Time::unscaledDeltaTime * 1000, 2);
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "Window size: " << renderer->GetWindow().GetWidth() << "x" << renderer->GetWindow().GetHeight();
	ImGui::Text(ss.str().c_str());
	ss.str("");


	ss.str("");
	ss << "Platform: " << renderer->GetWindow().GetWindowInfo().Platform;
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "Renderer: " << renderer->GetWindow().GetWindowInfo().Renderer;
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "Draw calls: " << Hazard::HazardLoop::GetAppInfo().Get("DrawCalls");
	ImGui::Text(ss.str().c_str());

	ImGui::NewLine();

	ImGui::Text("\nActive modules: ");
	for (Hazard::Module* module : Hazard::ModuleHandler::GetAll()) {
		ss.str("");
		ss << "- " << module->GetName();
		ImGui::Text(ss.str().c_str());
	}
	ImGui::End();
}
