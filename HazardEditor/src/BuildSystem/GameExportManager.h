#if 0
#pragma once

#include "hzrpch.h"
#include "BuildReport.h"

namespace Exporter 
{
	using BuildErrorCallback = std::function<void()>;
	using namespace Hazard;

	struct BuildSettings {
		std::vector<AssetMetadata> m_Worlds;
	};

	class GameExportManager {
	public:
		GameExportManager() = default;
		~GameExportManager() = default;

		void SetBuildSettings(BuildSettings settings) { m_Settings = settings; };
		BuildSettings GetSettings() { return m_Settings; }
		bool SaveBuildSettings(const std::string& path, const BuildSettings& settings);
		void SetErrorCallback(BuildErrorCallback callback) { m_ErrorCallback = callback; }
		
		bool ValidateBuildSettings();
		bool ExportGame(const std::filesystem::path& location);

		BuildReport GetBuildReport() { return m_Report; }

	private:
		BuildSettings m_Settings;
		BuildReport m_Report;
		BuildErrorCallback m_ErrorCallback;
	};
}
#endif