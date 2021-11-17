#pragma once

#include "Hazard.h"
#include "BuildReport.h"

namespace Exporter 
{
	using BuildErrorCallback = std::function<void()>;

	using namespace Hazard::ECS;
	struct BuildSettings {
		std::vector<Ref<World>> m_Worlds;
	};

	class GameExportManager {
	public:
		GameExportManager() = default;
		~GameExportManager() = default;

		void SetBuildSettings(BuildSettings settings) { m_Settings = settings; };
		BuildSettings GetSettings() { return m_Settings; }
		void SetErrorCallback(BuildErrorCallback callback) { m_ErrorCallback = callback; }
		
		bool ValidateBuildSettings() { return true; }
		bool ExportGame(const std::filesystem::path& location);

		BuildReport GetBuildReport() { return m_Report; }

	private:
		BuildSettings m_Settings;
		BuildReport m_Report;
		BuildErrorCallback m_ErrorCallback;
	};
}