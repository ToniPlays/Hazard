#pragma once

#include "Hazard/Assets/AssetPack.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"
#include "HazardRenderer.h"
#include <Hazard/Rendering/Mesh/MeshAssetLoader.h>
#include "GUI/AssetTools/AssetImporterPanel.h"

using namespace Hazard;
class EditorAssetPackBuilder
{
public:

	static AssetPack CreateAssetPack(const std::vector<AssetPackElement> element);
	static CachedBuffer AssetPackToBuffer(const AssetPack& pack);
	static void GenerateAndSaveAssetPack(Ref<Job> job, const std::filesystem::path& path);

	static void ImageAssetPackJob(Ref<Job> job, const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo info, UI::ImageImportSettings settings);
	static void MeshAssetPackJob(Ref<Job> job, const std::filesystem::path& file, MeshCreateInfo info, UI::MeshImportSettings settings);

	template<typename T>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, T info)
	{
		__debugbreak();
		return nullptr;
	}
	template<typename T, typename E>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, T info, E setting)
	{
		__debugbreak();	
		return nullptr;
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo info, UI::ImageImportSettings settings)
	{
		Ref<Job> job = Ref<Job>::Create(ImageAssetPackJob, file, info, settings);
		job->SetJobTag(file.string());

		Ref<JobGraph> graph = Ref<JobGraph>::Create(File::GetName(file), 1);
		graph->GetStage(0)->QueueJobs({ job });
		return graph;
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, HazardRenderer::RenderAPI renderApi)
	{
		if (!File::Exists(file)) return nullptr;

		using namespace HazardRenderer;
		std::vector<ShaderStageCode> binaries = ShaderCompiler::GetShaderBinariesFromSource(file, renderApi);

		size_t codeSize = 0;
		for (auto& stage : binaries)
			codeSize += sizeof(ShaderStage) + sizeof(uint32_t) + stage.ShaderCode.Size;

		Buffer data;
		//Header and combined binary size
		data.Allocate(codeSize);

		size_t offset = 0;

		for (auto& stage : binaries)
		{
			data.Write(&stage.Stage, sizeof(uint32_t), offset);
			offset += sizeof(uint32_t);
			data.Write(&stage.Size, sizeof(uint32_t), offset);
			offset += sizeof(uint32_t);
			data.Write(stage.ShaderCode.Data, stage.ShaderCode.Size, offset);
			offset += stage.ShaderCode.Size;
		}

		AssetPackElement result = {};
		result.Type = AssetType::Shader;
		result.Data = data;

		return Ref<JobGraph>::Create("No", 0);
	}
	template<>
	static Ref<JobGraph> CreatePackElement(const std::filesystem::path& file, MeshCreateInfo info, UI::MeshImportSettings settings)
	{
		if (!File::Exists(file))
			return nullptr;
	
		Ref<Job> job = Ref<Job>::Create(MeshAssetPackJob, file, info, settings);
		job->SetJobTag(file.string());

		Ref<JobGraph> graph = Ref<JobGraph>::Create(File::GetName(file), 1);
		graph->GetStage(0)->QueueJobs({ job });

		return graph;
	}
};