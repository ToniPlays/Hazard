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
	Ref<JobGraph> EnvironmentAssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		if (!File::Exists(metadata.FilePath))
		{
			HZR_CORE_ERROR("File does not exist");
			return nullptr;
		}

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
			.Stages = { { "Save", 1.0f, { saveJob } },
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

		Buffer buf = Buffer::Copy(&create, sizeof(CreateSettings));
		Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create(buf);
		info.Job->SetResult(buffer);
	}
	void EnvironmentAssetLoader::CreateImageFromSource(JobInfo& info, const std::filesystem::path& sourcePath)
	{
		using namespace HazardRenderer;
		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(sourcePath, true);

		if (!header.ImageData)
			throw JobException("Data not loaded");

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
			.DebugName = fmt::format("Env map {}", settings.Resolution),
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

		ImageMemoryInfo barrier = {
			.Image = cubemap,
			.BaseLayer = 0,
			.LayerCount = 6,
			.BaseMip = 0,
			.MipCount = cubemap->GetMipLevels(),
			.SrcLayout = IMAGE_LAYOUT_SHADER_READ_ONLY,
			.DstLayout = IMAGE_LAYOUT_GENERAL,
		};

		cmdBuffer->Begin();
		cmdBuffer->ImageMemoryBarrier(barrier);

		computeSet->Write(0, 0, cubemap, RenderEngine::GetResources().DefaultImageSampler, true);
		computeSet->Write(1, 0, image, RenderEngine::GetResources().DefaultImageSampler, true);

		cmdBuffer->SetPipeline(pipeline);
		cmdBuffer->SetDescriptorSet(computeSet, 0);
		cmdBuffer->DispatchCompute({ settings.Resolution / 32, settings.Resolution / 32, 6 });

		barrier.SrcLayout = IMAGE_LAYOUT_GENERAL;
		barrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;

		cmdBuffer->ImageMemoryBarrier(barrier);
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
		asset->m_RadianceMap = cubemap;
		asset->m_Spec.Resolution = cubemap->GetExtent().Width;
		asset->m_Spec.Samples = samples;

		asset->Invalidate();

		info.Job->SetResult(asset);
	}
}
