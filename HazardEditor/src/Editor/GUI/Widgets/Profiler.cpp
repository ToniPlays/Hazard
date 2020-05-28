#pragma once
#include "Profiler.h"
#include "imgui.h"
#include <sstream>

bool Profiler::layerOpen = true;

Profiler::Profiler()
{

}


bool Profiler::OnEnabled()
{
	Hazard::Logger* logger = Hazard::ModuleHandler::GetModule<Hazard::Logger>();
	return logger != nullptr;
}

void Profiler::Render()
{
	if (!layerOpen) return;

	ImGui::Begin("Profiler", &layerOpen);
	
	Hazard::Logger* logger = Hazard::ModuleHandler::GetModule<Hazard::Logger>();
	std::stringstream ss;
	for (std::pair<std::string, Hazard::ProfiledFunction> profiled : logger->GetLogs()) {
		ss << profiled.first << ": " << profiled.second.curVal << "ms";

		ImGui::Text(ss.str().c_str());
		ss.str("");
	}

	ImGui::End();
}
