#pragma once

#include "Project/ProjectSettingsSchema.h"
#include "Jobs.h"

class RuntimeExporter
{
public:
	RuntimeExporter(Platform platform) : m_Platform(platform) {};
	~RuntimeExporter() = default;

	void SetTargetWorlds(std::vector<Hazard::AssetMetadata> metadata)
	{
		m_TargetWorlds = metadata;
	}
	void Export(const std::filesystem::path& target);

private:
	std::vector<Ref<Job>> GetDependencyJobs();
	std::vector<Ref<Job>> GetSaveWorldJobs();
	
	static void GetAssetDependenciesJob(JobInfo& job, const std::filesystem::path& sourcePath);
	static void DispatchAssetJobs(JobInfo& job, std::filesystem::path& assetPath);
	static void SaveWorldAssetJob(JobInfo& job, std::filesystem::path& assetPath, AssetHandle handle);

private:
	Platform m_Platform;
	std::filesystem::path m_BuildTarget;
	std::vector<Hazard::AssetMetadata> m_TargetWorlds;
};