#pragma once
#include <hzreditor.h>
#include "Profiler.h"

using namespace Hazard;

Profiler::Profiler() : Layer("Profiler") {}

bool Profiler::OnEnabled()
{
	Logger* logger = ModuleHandler::GetModule<Logger>();
	return logger != nullptr;
}

void Profiler::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;
	
	Logger* logger = ModuleHandler::GetModule<Logger>();
	std::stringstream ss;
	for (std::pair<std::string, ProfiledFunction> profiled : logger->GetLogs()) {
		ss << profiled.first << ": " << profiled.second.curVal << "ms";

		ImGui::Text(ss.str().c_str());
		ss.str("");
	}

	Panel::End();
}
