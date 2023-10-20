#pragma once

#include "Hazard/Assets/AssetPack.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"
#include "HazardRenderer.h"
#include <Hazard/Rendering/Mesh/MeshAssetLoader.h>
#include "GUI/AssetTools/AssetImporterPanel.h"
#include <Hazard/Rendering/Mesh/MaterialAssetLoader.h>

//TODO: Get rid of this mostly

using namespace Hazard;
class EditorAssetPackBuilder
{
public:
	static AssetPack CreateAssetPack(const std::vector<AssetPackElement> element);
	static void GenerateAndSaveAssetPack(Ref<Job> job, const std::filesystem::path& path);

	static void ImageAssetPackJob(Ref<Job> job, const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo info, UI::ImageImportSettings settings);
	static void MeshAssetPackJob(Ref<Job> job, const std::filesystem::path& file, MeshCreateInfo info, UI::MeshImportSettings settings);
	static void ShaderAssetPackJob(Ref<Job> job, const std::filesystem::path& file, HazardRenderer::RenderAPI api);
	static void MaterialAssetPackJob(Ref<Job> job, const std::filesystem::path& file, UI::MaterialImportSettings settings);

	template<typename T>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, T info)
	{
		return nullptr;
	}
	template<typename T, typename E>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, T info, E setting)
	{
		return nullptr;
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo info, UI::ImageImportSettings settings)
	{
		Ref<Job> job = Ref<Job>::Create(fmt::format("{0}", file.string()), ImageAssetPackJob, file, info, settings);
		job->SetJobTag(file.string());

		Ref<JobGraph> graph = Ref<JobGraph>::Create(File::GetName(file), 1);
		graph->GetStage(0)->QueueJobs({ job });
		return graph;
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, HazardRenderer::RenderAPI renderApi)
	{
		if (!File::Exists(file))
			return nullptr;

		Ref<Job> job = Ref<Job>::Create(fmt::format("{0}", file.string()), ShaderAssetPackJob, file, renderApi);
		job->SetJobTag(file.string());

		Ref<JobGraph> graph = Ref<JobGraph>::Create(File::GetName(file), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, MeshCreateInfo info, UI::MeshImportSettings settings)
	{
		if (!File::Exists(file))
			return nullptr;
	
		Ref<Job> job = Ref<Job>::Create(fmt::format("{0}", file.string()), MeshAssetPackJob, file, info, settings);
		job->SetJobTag(file.string());

		Ref<JobGraph> graph = Ref<JobGraph>::Create(File::GetName(file), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, UI::MaterialImportSettings settings)
	{
		if (!File::Exists(file))
			return nullptr;

		Ref<Job> job = Ref<Job>::Create(fmt::format("{0}", file.string()), MaterialAssetPackJob, file, settings);
		job->SetJobTag(file.string());

		Ref<JobGraph> graph = Ref<JobGraph>::Create(File::GetName(file), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
};
