#pragma once
#include "Profiler.h"
#include "HzrEditor.h"

bool Profiler::layerOpen = true;

void Profiler::OnRender() const
{
	if (!layerOpen) return;

	ImGui::Begin("Profiler", &layerOpen);
	
	Hazard::Logger* logger = Hazard::Application::GetCurrent().GetModuleHandler().GetModule<Hazard::Logger>();
	std::unordered_map<std::string, Hazard::ProfiledFunction> logs = logger->GetLogs();

	for (std::pair<std::string, Hazard::ProfiledFunction> log : logs) {
		std::stringstream ss;
		ss << log.first << ": " << log.second.time << " ms";
		ImGui::Text(ss.str().c_str());
	}
	ImGui::End();
}

void Profiler::OpenLayer() const
{

}

void Profiler::CloseLayer() const
{

}
