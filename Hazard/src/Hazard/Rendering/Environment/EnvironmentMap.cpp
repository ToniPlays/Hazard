#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Backend/Core/Texture/Texture.h"

#include "Hazard/RenderContext/ShaderLibrary.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		m_Handle = UID();
		m_Flags = AssetFlags::RuntimeGenerated;
		m_Type = AssetType::EnvironmentMap;

		BRDFLut = AssetManager::GetAsset<Texture2DAsset>("res/Textures/BRDF_LUT.tga");
	}
	void EnvironmentMap::GenerateRadiance(Ref<Texture2DAsset> sourceImage)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
		Ref<Image2D> image = sourceImage->GetSourceImageAsset()->Value.As<Image2D>();
		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", 1, true);
		Ref<RenderCommandBuffer> graphicsBuffer = RenderCommandBuffer::Create("Transition", 1);

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap " + image->GetDebugName();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.Width = 4096;
		radianceInfo.Height = 4096;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);

		Ref<Pipeline> computePipeline = ShaderLibrary::GetPipeline("EquirectangularToCubeMap");

		auto shader = computePipeline->GetShader();
		shader->Set("o_CubeMap", 0, radianceMap);
		shader->Set("u_EquirectangularTexture", 0, image);

		DispatchComputeInfo computeInfo = {};
		computeInfo.GroupSize = { radianceInfo.Width / 32, radianceInfo.Height / 32, 6};
		computeInfo.Pipeline = computePipeline;
		computeInfo.WaitForCompletion = true;

		ImageTransitionInfo shaderLayout = {};
		shaderLayout.Cubemap = radianceMap;
		shaderLayout.SourceLayout = ImageLayout_General;
		shaderLayout.DestLayout = ImageLayout_ShaderReadOnly;

		GenMipmapsInfo mipmapsInfo = {};
		mipmapsInfo.Cubemap = radianceMap;

		computeBuffer->Begin();
		computeBuffer->DispatchCompute(computeInfo);
		computeBuffer->End();
		computeBuffer->Submit();

		graphicsBuffer->Begin();
		graphicsBuffer->GenerateMipmaps(mipmapsInfo);
		graphicsBuffer->End();
		graphicsBuffer->Submit();

		RadianceMap = AssetPointer::Create(radianceMap, AssetType::EnvironmentMap);
		AssetMetadata radianceMetadata = {};
		radianceMetadata.Handle = RadianceMap->GetHandle();
		radianceMetadata.Type = AssetType::EnvironmentMap;

		AssetManager::AddRuntimeAsset(radianceMetadata, RadianceMap);
		SourceImage = sourceImage;
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
		Ref<Pipeline> irradiancePipeline = ShaderLibrary::GetPipeline("EnvironmentIrradiance");

		irradiancePipeline->GetShader()->Set("o_IrradianceMap", 0, irradianceMap);
		irradiancePipeline->GetShader()->Set("u_RadianceMap", 0, radianceMap->Value.As<CubemapTexture>());

		DispatchComputeInfo computeInfo = {};
		computeInfo.GroupSize = { irradianceInfo.Width / 32, irradianceInfo.Height / 32, 6 };
		computeInfo.Pipeline = irradiancePipeline;
		computeInfo.WaitForCompletion = true;

		GenMipmapsInfo mipmapInfo = {};
		mipmapInfo.Cubemap = irradianceMap;

		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", 1, true);
		Ref<RenderCommandBuffer> graphicsBuffer = RenderCommandBuffer::Create("Transition", 1);

		computeBuffer->Begin();
		computeBuffer->DispatchCompute(computeInfo);
		computeBuffer->End();
		computeBuffer->Submit();

		graphicsBuffer->Begin();
		graphicsBuffer->GenerateMipmaps(mipmapInfo);
		graphicsBuffer->End();
		graphicsBuffer->Submit();

		IrradianceMap = AssetPointer::Create(irradianceMap, AssetType::EnvironmentMap);

		AssetMetadata irradianceMetadata = {};
		irradianceMetadata.Handle = IrradianceMap->GetHandle();
		irradianceMetadata.Type = AssetType::EnvironmentMap;
		AssetManager::AddRuntimeAsset(irradianceMetadata, IrradianceMap);
	}
	void EnvironmentMap::GeneratePreFilter(Ref<AssetPointer> radiance)
	{
		/*
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		//Generate prefilter map
		PipelineSpecification preFilterPipelineInfo = {};
		preFilterPipelineInfo.DebugName = "EnvironmentMipFilter";
		preFilterPipelineInfo.ShaderPath = "res/Shaders/Compute/EnvironmentMipFilter.glsl";
		preFilterPipelineInfo.Usage = PipelineUsage::ComputeBit;

		Ref<Pipeline> preFilterPipeline = Pipeline::Create(&preFilterPipelineInfo);
		preFilterPipeline->GetShader()->Set("u_InputCube", 0, radiance->Value.As<CubemapTexture>());

		CubemapGen preFilterSource = {};
		preFilterSource.OutputImageName = "o_Texture";
		preFilterSource.pCubemap = radiance->Value.As<CubemapTexture>();
		preFilterSource.Pipeline = preFilterPipeline;

		CubemapTextureCreateInfo prefilterMapInfo = {};
		prefilterMapInfo.DebugName = "PrefilterMap";
		prefilterMapInfo.Usage = ImageUsage::Texture;
		prefilterMapInfo.pCubemapSrc = &preFilterSource;
		prefilterMapInfo.Format = ImageFormat::RGBA;
		prefilterMapInfo.GenerateMips = true;
		Ref<CubemapTexture> preFilterMap = CubemapTexture::Create(&prefilterMapInfo);

		PreFilterMap = AssetPointer::Create(preFilterMap, AssetType::EnvironmentMap);

		AssetMetadata prefilterMetadata = {};
		prefilterMetadata.Handle = PreFilterMap->GetHandle();
		prefilterMetadata.Type = AssetType::EnvironmentMap;
		AssetManager::AddRuntimeAsset(prefilterMetadata, PreFilterMap);
		*/
	}
	Ref<EnvironmentMap> EnvironmentMap::Create(Ref<Texture2DAsset> sourceImage)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		Ref<EnvironmentMap> map = Ref<EnvironmentMap>::Create();
		if (!sourceImage) return map;

		map->GenerateRadiance(sourceImage);
		map->GenerateIrradiance(map->RadianceMap);
		//map->GeneratePreFilter(map->IrradianceMap);
		return map;
	}
}