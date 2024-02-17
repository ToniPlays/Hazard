#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/Rendering/RenderEngine.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		Ref<Pipeline> skyboxPipeline = ShaderLibrary::GetPipeline("Skybox");
		m_Material = Ref<Material>::Create(skyboxPipeline);
		m_IrradianceMap = RenderEngine::GetResources().BlackCubemap;
	}

	void EnvironmentMap::Invalidate()
	{
		GenerateIrradiance();
	}

	void EnvironmentMap::GenerateIrradiance()
	{
		using namespace HazardRenderer;

		//Swap default cubemap
		if (m_IrradianceMap->GetExtent().Height == 1)
		{
			CubemapCreateInfo cubemapInfo = {
				.DebugName = "Irradiance",
				.Width = 32,
				.Height = 32,
				.MaxMips = 1,
				.Usage = ImageUsage::Texture,
				.Format = ImageFormat::RGBA,
			};

			m_IrradianceMap = Cubemap::Create(&cubemapInfo);
		}

		DescriptorSetLayout layout = {
			{ SHADER_STAGE_COMPUTE_BIT, "o_IrradianceMap", 0, 1, DESCRIPTOR_TYPE_STORAGE_IMAGE },
			{ SHADER_STAGE_COMPUTE_BIT, "u_RadianceMap", 1, 1, DESCRIPTOR_TYPE_SAMPLER_CUBE },
		};

		DescriptorSetCreateInfo setInfo = {
			.DebugName = "Irradiance compute",
			.Set = 0,
			.pLayout = &layout,
		};

		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("Irradiance compute buffer", DeviceQueue::ComputeBit, 1);
		Ref<Pipeline> compute = ShaderLibrary::GetPipeline("EnvironmentIrradiance");
		Ref<DescriptorSet> descriptor = DescriptorSet::Create(&setInfo);

	
		uint32_t samples = 16;

		ImageMemoryInfo imageBarrier = {
			.Image = m_IrradianceMap.As<Image>(),
			.BaseLayer = 0,
			.LayerCount = 6,
			.BaseMip = 0,
			.MipCount = m_IrradianceMap->GetMipLevels(),
			.SrcLayout = IMAGE_LAYOUT_SHADER_READ_ONLY,
			.DstLayout = IMAGE_LAYOUT_GENERAL,
		};

		computeBuffer->Begin();
		computeBuffer->ImageMemoryBarrier(imageBarrier);

		descriptor->Write(0, 0, m_IrradianceMap.As<Image>(), RenderEngine::GetResources().DefaultImageSampler, false);
		descriptor->Write(1, 0, m_RadianceMap.As<Image>(), RenderEngine::GetResources().DefaultImageSampler, false);

		computeBuffer->SetPipeline(compute);
		computeBuffer->SetDescriptorSet(descriptor, 0);
		computeBuffer->PushConstants({ &samples, sizeof(uint32_t) }, 0, SHADER_STAGE_COMPUTE_BIT);
		computeBuffer->DispatchCompute({ m_IrradianceMap->GetExtent().Width / 32, m_IrradianceMap->GetExtent().Height / 32, 6 });

		imageBarrier.SrcLayout = IMAGE_LAYOUT_GENERAL;
		imageBarrier.DstLayout = IMAGE_LAYOUT_SHADER_READ_ONLY;

		computeBuffer->ImageMemoryBarrier(imageBarrier);
		computeBuffer->End();
		computeBuffer->Submit();
	}
}
