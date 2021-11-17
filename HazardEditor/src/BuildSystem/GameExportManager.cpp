#pragma once
#include "GameExportManager.h"

namespace Exporter
{
	bool GameExportManager::SaveBuildSettings(const std::string& path, const BuildSettings& settings)
	{
		using namespace Hazard;
		YAML::Emitter out;

		YamlUtils::Sequence(out, "World", [&]() {
			for (auto world : settings.m_Worlds) {
				out << YAML::BeginMap;
				YamlUtils::Serialize(out, "World", world->GetName());
				out << YAML::EndMap;
			}
			});

		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
		return true;
	}
	bool GameExportManager::ValidateBuildSettings()
	{
		return true;
	}
	bool GameExportManager::ExportGame(const std::filesystem::path& location)
	{
		using namespace std::chrono_literals;
		using namespace Hazard;
		Timer timer;

		m_Report = BuildReport();
		m_Report.AddMessage({ "Build has started", "GameExport", BuildMessageSeverity::Info });
		m_Report.TotalSize = 4095;

		m_Report.AddResource({ AssetType::World, "World1", 235324 });
		m_Report.AddResource({ AssetType::World, "World2", 4675465 });
		m_Report.AddResource({ AssetType::Image, "Texture2D", 465364 });
		m_Report.AddResource({ AssetType::Image, "Texture32D", 345444 });
		m_Report.AddResource({ AssetType::EnvironmentMap, "HDRSkyBox", 532532 });
		m_Report.AddResource({ AssetType::Mesh, "C8_Corvette", 3253534 });

		std::this_thread::sleep_for(200ms);

		m_Report.BuildTime = timer.ElapsedMillis();
		m_Report.Result = true;
		return true;
	}
}