#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Core/Rendering/Texture.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		m_Handle = UID();
		m_Type = AssetType::EnvironmentMap;

		BRDFLut = AssetManager::GetAsset<Texture2DAsset>("res/Textures/BRDF_LUT.tga");
	}

	EnvironmentMap::~EnvironmentMap()
	{

	}

	void EnvironmentMap::Update(uint32_t samples, uint32_t resolution, AssetHandle sourceImage)
	{
		m_SourceImageHandle = sourceImage;
		m_Spec.Samples = samples;
		m_Spec.Resolution = resolution;
	}

	void EnvironmentMap::Invalidate() 
	{
		if (m_SourceImageHandle != INVALID_ASSET_HANDLE)
			GenerateRadiance();
	}

	void EnvironmentMap::GenerateRadiance()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
		Ref<Texture2DAsset> sourceImage = AssetManager::GetAsset<Texture2DAsset>(m_SourceImageHandle);
		if (!sourceImage) return;

		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", DeviceQueue::ComputeBit, 1);

		Ref<Image2D> image = sourceImage->GetSourceImageAsset()->Value.As<Image2D>();

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap " + image->GetDebugName();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.Width = m_Spec.Resolution;
		radianceInfo.Height = m_Spec.Resolution;
		radianceInfo.GenerateMips = false;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);
		AssetHandle computePipelineHandle = ShaderLibrary::GetPipelineAssetHandle("EquirectangularToCubemap");
		Ref<Pipeline> computePipeline = AssetManager::GetAsset<AssetPointer>(computePipelineHandle)->Value.As<Pipeline>();

		/*DescriptorSetLayout layout = {{"o_CubeMap", 0, DESCRIPTOR_TYPE_STORAGE_IMAGE},
									   { "u_EquirectangularTexture", 1, DESCRIPTOR_TYPE_SAMPLER_2D } };
									   */

		DescriptorSetCreateInfo setInfo = {};
		setInfo.DebugName = "EquirectangularComputeSet";
		setInfo.Set = 0;
		//setInfo.pLayout = &layout;

		Ref<DescriptorSet> set = DescriptorSet::Create(&setInfo);
		set->Write(0, 0, radianceMap, nullptr, true);
		set->Write(1, 0, image, RenderEngine::GetResources().DefaultImageSampler, true);

		GroupSize size = { radianceInfo.Width / 32, radianceInfo.Height / 32, 6 };

		//Command buffer recording
		computeBuffer->Begin();
		computeBuffer->SetPipeline(computePipeline);
		computeBuffer->SetDescriptorSet(set, 0);
		computeBuffer->DispatchCompute(size);
		computeBuffer->End();
		computeBuffer->Submit();

		RadianceMap = AssetPointer::Create(radianceMap, AssetType::EnvironmentMap);
		AssetManager::CreateMemoryOnly(AssetType::EnvironmentMap, RadianceMap);
	}

	void EnvironmentMap::GenerateIrradiance(Ref<AssetPointer> radianceMap)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		CubemapTextureCreateInfo irradianceInfo = {};
		irradianceInfo.DebugName = "IrradianceMap";
		irradianceInfo.Width = 32;
		irradianceInfo.Height = 32;
		irradianceInfo.Usage = ImageUsage::Texture;
		irradianceInfo.Format = ImageFormat::RGBA;
		irradianceInfo.GenerateMips = true;

		Ref<CubemapTexture> irradianceMap = CubemapTexture::Create(&irradianceInfo);

		AssetHandle irradiancePipelineHandle = ShaderLibrary::GetPipelineAssetHandle("EnvironmentIrradiance");
		Ref<Pipeline> irradiancePipeline = AssetManager::GetAsset<AssetPointer>(irradiancePipelineHandle)->Value.As<Pipeline>();

		{
			auto shader = irradiancePipeline->GetShader();
			//shader->Set("o_IrradianceMap", 0, irradianceMap);
			//shader->Set("u_RadianceMap", 0, radianceMap->Value.As<CubemapTexture>());
			//shader->Set("u_Settings", Buffer(&m_Samples, sizeof(uint32_t)));
		}

		GroupSize size = { irradianceInfo.Width / 32, irradianceInfo.Height / 32, 6 };

		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", DeviceQueue::ComputeBit, 1);

		computeBuffer->Begin();
		computeBuffer->SetPipeline(irradiancePipeline);
		computeBuffer->DispatchCompute(size);
		computeBuffer->End();
		computeBuffer->Submit();

		IrradianceMap = AssetPointer::Create(irradianceMap, AssetType::EnvironmentMap);
		AssetManager::CreateMemoryOnly(AssetType::EnvironmentMap, IrradianceMap);
	}

	void EnvironmentMap::GeneratePreFilter(Ref<AssetPointer> radiance)
	{
		HZR_PROFILE_FUNCTION();
	}
}
