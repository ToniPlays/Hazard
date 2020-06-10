#pragma once
#include "Performance.h"
#include "imgui.h"
#include <sstream>

bool Performance::layerOpen = true;
static bool vsync;
Performance::Performance() {}

bool Performance::OnEnabled()
{
	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();
	if(renderer != nullptr)
		vsync = renderer->GetWindow().IsVSync();
	return true;
}

void Performance::Render()
{
	if (!layerOpen) return;

	ImGui::Begin("Performance", &layerOpen);

	Hazard::GlobalRenderer* renderer = Hazard::ModuleHandler::GetModule<Hazard::GlobalRenderer>();

	ImGui::SliderFloat("Rotation speed ", &Hazard::GlobalRenderer::speed, 0.0f, 360.0f);

	ImGui::NewLine();
	ImGui::Checkbox("VSync", &vsync);

	renderer->GetWindow().SetVSync(vsync);

	std::stringstream ss;
	ss << "Frametime " << Hazard::Time::unscaledDeltaTime * 1000 << "ms";
	ImGui::Text(ss.str().c_str());
	ss.str("");

	ss << "FPS " << Hazard::Math::Round(1 / Hazard::Time::unscaledDeltaTime, 2);
	ImGui::Text(ss.str().c_str());

	ss.str("");
	ss << "Window size: " << renderer->GetWindow().GetWidth() << "x" << renderer->GetWindow().GetHeight();
	ss << ", focus " << (renderer->GetWindow().IsFocused() ? "True" : "False");
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
