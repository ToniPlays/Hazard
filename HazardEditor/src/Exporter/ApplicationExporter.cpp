#include "ApplicationExporter.h"
#include "Directory.h"


namespace Exporter
{
	void ApplicationExporter::Export(const ExportSettings& settings)
	{
		m_Settings = settings;

		HZR_INFO("Exporting to: {0}", settings.TargetPath.string());

		Directory::Create(settings.TargetPath);
		m_ExportGraph = CreateExportJob();

		Hazard::Application::Get().GetJobSystem().QueueGraph(m_ExportGraph);
	}

	bool ApplicationExporter::IsExporting()
	{
		if (!m_ExportGraph) return false;
		return !m_ExportGraph->HasFinished();
	}

	Ref<JobGraph> ApplicationExporter::CreateExportJob()
	{
		/*Ref<JobGraph> graph = Ref<JobGraph>::Create("Export", 1);
		
		for (AssetHandle handle : m_Settings.WorldHandles)
		{
			HZR_WARN("Preprocessing world {0}", handle);
			Ref<Job> job = Ref<Job>::Create(fmt::format("{0} preprocess", handle), PreprocessWorldJob, handle);

			graph->GetStage(0)->QueueJobs({ job });
		}

		return graph;
		*/
		return nullptr;
	}
	void ApplicationExporter::PreprocessWorldJob(JobInfo& info, Hazard::AssetHandle handle)
	{

	}
}


