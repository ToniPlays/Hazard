#pragma once
#include "GameExportManager.h"

namespace Exporter 
{
	bool GameExportManager::ExportGame(const std::filesystem::path& location)
	{
		using namespace std::chrono_literals;
		using namespace Hazard;
		Timer timer;

		m_Report = BuildReport();
		m_Report.AddMessage({ "Build has started", "GameExport", BuildMessageSeverity::Info });
		m_Report.TotalSize = 1024;

		std::this_thread::sleep_for(200ms);

		m_Report.BuildTime = timer.ElapsedMillis();
		m_Report.Result = true;
		return true;
	}
}