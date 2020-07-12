#pragma once
#include <hzreditor.h>
#include "Profiler.h"


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
	if (!isLayerOpen) return;

	ImGui::Begin("Profiler", &isLayerOpen);
	
	Hazard::Logger* logger = Hazard::ModuleHandler::GetModule<Hazard::Logger>();
	std::stringstream ss;
	for (std::pair<std::string, Hazard::ProfiledFunction> profiled : logger->GetLogs()) {
		ss << profiled.first << ": " << profiled.second.curVal << "ms";

		ImGui::Text(ss.str().c_str());
		ss.str("");
	}

	ImGui::End();
}
