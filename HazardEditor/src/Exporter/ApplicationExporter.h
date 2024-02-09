#pragma once

#include "Hazard.h"
#include "Jobs.h"

namespace Exporter
{
	struct ExportSettings 
	{
		std::filesystem::path TargetPath;
		std::vector<Hazard::AssetHandle> WorldHandles;
	};

	class ApplicationExporter
	{
	public:
		ApplicationExporter() = default;
		~ApplicationExporter() = default;

		void Export(const ExportSettings& path);
		bool IsExporting();

	private:
		Ref<JobGraph> CreateExportJob();

		static void PreprocessWorldJob(JobInfo& info, Hazard::AssetHandle handle);

	private:
		ExportSettings m_Settings;
		Ref<JobGraph> m_ExportGraph = nullptr;
	};
}