#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "EnvironmentMap.h"
#include <Hazard/RenderContext/ShaderLibrary.h>
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/Assets/AssetPack.h"

namespace Hazard
{
	Ref<JobGraph> EnvironmentAssetLoader::Load(AssetMetadata& metadata)
	{
		if (!File::Exists(metadata.FilePath))
			return nullptr;

		Ref<CachedBuffer> data = File::ReadBinaryFile(metadata.FilePath);

		AssetPack pack = {};
		pack.FromBuffer(data);

		CreateSettings create = pack.AssetData->Read<CreateSettings>();

		Ref<Job> loadJob = Ref<Job>::Create("Environment map source load", CreateImageFromSource, metadata.SourceFile);
		Ref<Job> genJob = Ref<Job>::Create("Environment map generate", GenerateEnvironmentMap, create);
		Ref<Job> createJob = Ref<Job>::Create("Create", CreateEnvironmentAsset, create.Samples);

		JobGraphInfo info = {
			.Name = "Environment map load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Load", 0.3f, { loadJob } },
						{ "Convert", 0.5f, { genJob } },
						{ "Finalize", 0.2f, { createJob } }
			}
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> EnvironmentAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> saveJob = Ref<Job>::Create("Environment map save", SaveEnvironmentAsset, asset.As<EnvironmentMap>());

		JobGraphInfo info = {
			.Name = "Environment map save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Save", 0.3f, { saveJob } },
			}
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> EnvironmentAssetLoader::Create(const CreateAssetSettings& settings)
	{
		HZR_CORE_ASSERT(!settings.SourcePath.empty(), "Source path cannot be empty");

		CreateSettings create = {};
		if (settings.Settings)
			create = *(CreateSettings*)settings.Settings;

		Ref<Job> loadJob = Ref<Job>::Create("Environment map source load", CreateImageFromSource, settings.SourcePath);
		Ref<Job> genJob = Ref<Job>::Create("Environment map generate", GenerateEnvironmentMap, create);
		Ref<Job> createJob = Ref<Job>::Create("Create", CreateEnvironmentAsset, create.Samples);

		JobGraphInfo info = {
			.Name = "Environment map create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Load", 0.3f, { loadJob } },
						{ "Convert", 0.5f, { genJob } },
						{ "Create", 0.2f, { createJob } }
			}
		};

		return Ref<JobGraph>::Create(info);
	}
	void EnvironmentAssetLoader::SaveEnvironmentAsset(JobInfo& info, Ref<EnvironmentMap> map)
	{
		CreateSettings create = {
			.Resolution = map->GetSpec().Resolution,
			.Samples = map->GetSpec().Samples,
		};

		Buffer buffer = Buffer::Copy(&create, sizeof(CreateSettings));
		info.Job->SetResult(buffer);
	}
	void EnvironmentAssetLoader::CreateImageFromSource(JobInfo& info, const std::filesystem::path& sourcePath)
	{
		using namespace HazardRenderer;
		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(sourcePath, true);

		Image2DCreateInfo sourceImage = {
			.DebugName = fmt::format("Env map source: {}", sourcePath.string()),
			.Usage = ImageUsage::Texture,
			.Format = ImageFormat::RGBA32F,
			.Extent = header.Extent,
			.MaxMips = 1,
			.Data = header.ImageData,
		};

		Ref<Image2D> image = Image2D::Create(&sourceImage);
		info.Job->SetResult(image);

		header.ImageData.Release();
	}
	void EnvironmentAssetLoader::GenerateEnvironmentMap(JobInfo& info, const CreateSettings& settings)
	{
		using namespace HazardRenderer;

		CubemapCreateInfo cubemapSpec = {
			.DebugName = fmt::format("Env map"),
			.Width = settings.Resolution,
			.Height = settings.Resolution,
			.MaxMips = 64,
			.Usage = ImageUsage::Texture,
			.Format = ImageFormat::RGBA16F,
		};

		DescriptorSetLayout layout = { { SHADER_STAGE_COMPUTE_BIT, "o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE},
									   { SHADER_STAGE_COMPUTE_BIT, "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_2D } };

		DescriptorSetCreateInfo setInfo = {
			.DebugName = "EquirectangularComputeSet",
			.Set = 0,
			.pLayout = &layout,
		};

		Ref<Image2D> image = info.ParentGraph->GetResult<Ref<Image2D>>();
		Ref<Cubemap> cubemap = Cubemap::Create(&cubemapSpec);
		Ref<Pipeline> pipeline = ShaderLibrary::GetPipeline("EquirectangularToCubemap");
		Ref<DescriptorSet> computeSet = DescriptorSet::Create(&setInfo);
		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Equirectangular to cubemap", DeviceQueue::ComputeBit, 1);

		struct ComputeSettings
		{
			uint32_t Samples;
		};

		ComputeSettings constants = {
			.Samples = settings.Samples
		};

		computeSet->Write(0, 0, cubemap, RenderEngine::GetResources().DefaultImageSampler, true);
		computeSet->Write(1, 0, image, RenderEngine::GetResources().DefaultImageSampler, true);

		cmdBuffer->Begin();
		cmdBuffer->SetPipeline(pipeline);
		cmdBuffer->SetDescriptorSet(computeSet, 0);
		cmdBuffer->PushConstants(Buffer(&constants, sizeof(ComputeSettings)), 0, SHADER_STAGE_COMPUTE_BIT);
		cmdBuffer->DispatchCompute({ settings.Resolution / 32, settings.Resolution / 32, 6 });
		cmdBuffer->End();
		cmdBuffer->Submit();

		cubemap->RegenerateMips();
		info.Job->SetResult(cubemap);
	}
	void EnvironmentAssetLoader::CreateEnvironmentAsset(JobInfo& info, uint32_t samples)
	{
		using namespace HazardRenderer;
		Ref<Cubemap> cubemap = info.ParentGraph->GetResult<Ref<Cubemap>>();

		Ref<EnvironmentMap> asset = Ref<EnvironmentMap>::Create();
		asset->m_EnvironmentMap = cubemap;
		asset->m_Spec.Resolution = cubemap->GetExtent().Width;
		asset->m_Spec.Samples = samples;

		info.Job->SetResult(asset);
	}
}
