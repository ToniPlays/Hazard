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
	static bool realtime = logger->IsRealtime();

	if (ImGui::Checkbox("Realtime", &realtime)) {
		!logger->IsRealtime() ? logger->EnableRealtime() : logger->DisableRealtime();
	}
	if (logger->IsRealtime()) {
		for (std::pair<std::string, Hazard::ProfiledFunction> log : logs) {
			Hazard::Queue<double>* data = log.second.data;
			ImGui::PlotLines(log.first.c_str(), data->GetData(), data->GetSize(), 0, "", 0, 10);
		}
	}
	
	ImGui::End();
}

void Profiler::OpenLayer() const
{

}

void Profiler::CloseLayer() const
{

}
