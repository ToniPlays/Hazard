#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "EnvironmentMap.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/Assets/AssetPack.h"
#include <Hazard/RenderContext/ImageAssetLoader.h>
#include <Hazard/RenderContext/ShaderAsset.h>

namespace Hazard
{
	Ref<JobGraph> EnvironmentAssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		if (!File::Exists(metadata.FilePath))
		{
			HZR_CORE_ERROR("File does not exist");
			throw JobException("File not found");
		}

		Ref<CachedBuffer> data = File::ReadBinaryFile(metadata.FilePath);

		AssetPack pack = {};
		pack.FromBuffer(data);


		CreateSettings create = {};
		if (pack.AssetData->Available())
			create = pack.AssetData->Read<CreateSettings>();

		Ref<Job> loadJob = Job::Create("Environment map source load", CreateImageFromSource, metadata.SourceFile);
		Ref<Job> genJob = Job::Create("Environment map generate", GenerateEnvironmentMap, create);
		Ref<Job> createJob = Job::Create("Create", CreateEnvironmentAsset, create.Samples);

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
		Ref<Job> saveJob = Job::Create("Environment map save", SaveEnvironmentAsset, asset.As<EnvironmentMap>());

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
		CreateSettings create = {};
		if (settings.Settings)
			create = *(CreateSettings*)settings.Settings;

		Ref<Job> loadJob = Job::Create("Environment map source load", CreateImageFromSource, settings.SourcePath);
		Ref<Job> genJob = Job::Create("Environment map generate", GenerateEnvironmentMap, create);
		Ref<Job> createJob = Job::Create("Create", CreateEnvironmentAsset, create.Samples);

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

	Coroutine EnvironmentAssetLoader::SaveEnvironmentAsset(JobInfo info, Ref<EnvironmentMap> map)
	{
		CreateSettings create = {
			.Resolution = map->GetSpec().Resolution,
			.Samples = map->GetSpec().Samples,
		};

		Buffer buf = Buffer::Copy(&create, sizeof(CreateSettings));
		Ref<CachedBuffer> buffer = Ref<CachedBuffer>::Create(buf);
		info.Result(buffer);
		info.Current->Finish();
		co_return;
	}

	Coroutine EnvironmentAssetLoader::CreateImageFromSource(JobInfo info, const std::filesystem::path& sourcePath)
	{
		using namespace HazardRenderer;
		if (!File::Exists(sourcePath))
			co_return;
		
		Ref<Texture2DAsset> source = (co_await AssetManager::GetAssetAsync<Texture2DAsset>(sourcePath))[0];
		info.Result(source);
		info.Current->Finish();
	}

	Coroutine EnvironmentAssetLoader::GenerateEnvironmentMap(JobInfo info, const CreateSettings& settings)
	{
		using namespace HazardRenderer;

		Ref<Texture2DAsset> image = info.Graph->GetResults<Ref<Texture2DAsset>>()[0];
		if (!image)
			co_return;

		CubemapCreateInfo cubemapSpec = {
			.DebugName = fmt::format("Env map {}", settings.Resolution),
			.Size = settings.Resolution,
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

		Ref<Cubemap> cubemap = Cubemap::Create(&cubemapSpec);
		Ref<Pipeline> pipeline = AssetManager::GetAsset<ShaderAsset>("res/Shaders/Compute/EquirectangularToCubemap.shader")->GetPipeline();
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

		computeSet->Write(0, 0, cubemap, RenderContextManager::GetDefaultSampler(), true);
		computeSet->Write(1, 0, image->GetSourceImage(), RenderContextManager::GetDefaultSampler(), true);

		cmdBuffer->SetPipeline(pipeline);
		cmdBuffer->SetDescriptorSet(computeSet, 0);
		cmdBuffer->DispatchCompute({ settings.Resolution / 32, settings.Resolution / 32, 6 });

		barrier.SrcLayout = IMAGE_LAYOUT_GENERAL;
		barrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;

		cmdBuffer->ImageMemoryBarrier(barrier);
		cmdBuffer->End();
		cmdBuffer->Submit();

		cubemap->RegenerateMips();
		info.Result(cubemap);
		info.Current->Finish();
		co_return;
	}

	Coroutine EnvironmentAssetLoader::CreateEnvironmentAsset(JobInfo info, uint32_t samples)
	{
		using namespace HazardRenderer;
		Ref<Cubemap> cubemap = info.Graph->GetResults<Ref<Cubemap>>()[0];

		Ref<EnvironmentMap> asset = Ref<EnvironmentMap>::Create();
		asset->m_RadianceMap = cubemap;
		asset->m_Spec.Resolution = cubemap ? cubemap->GetExtent().Width : 2048;
		asset->m_Spec.Samples = samples;

		if (cubemap)
			asset->Invalidate();

		std::cout << "Create env" << std::endl;
		info.Result(asset);
		info.Current->Finish();
		co_return;
	}
}
