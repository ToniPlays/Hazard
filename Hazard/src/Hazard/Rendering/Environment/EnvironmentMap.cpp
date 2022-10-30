#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"

namespace Hazard
{
	EnvironmentMap::EnvironmentMap()
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		m_Handle = UID();
		m_Flags = AssetFlags::RuntimeGenerated;
		m_Type = AssetType::EnvironmentMap;

		AssetHandle brdfHandle = AssetManager::GetHandleFromFile("res/Textures/BRDF_LUT.tga");
		TextureHeader header = TextureFactory::GetFromCacheOrReload(brdfHandle, "res/Textures/BRDF_LUT.tga");

		Image2DCreateInfo brdfLutInfo = {};
		brdfLutInfo.DebugName = "BRDFLutTexture";
		brdfLutInfo.Width = header.Width;
		brdfLutInfo.Height = header.Height;
		brdfLutInfo.Data = header.ImageData;
		brdfLutInfo.Usage = ImageUsage::Texture;
		brdfLutInfo.Format = ImageFormat::RGBA;
		brdfLutInfo.GenerateMips = true;

		Ref<Image2D> lut = Image2D::Create(&brdfLutInfo);
		header.ImageData.Release();

		BRDFLut = AssetPointer::Create(lut, AssetType::Image);

		AssetMetadata BRDLutMetadata = {};
		BRDLutMetadata.Handle = BRDFLut->GetHandle();
		BRDLutMetadata.Type = AssetType::Image;
		AssetManager::AddRuntimeAsset(BRDLutMetadata, BRDFLut);
	}
	void EnvironmentMap::GenerateRadiance(Ref<Texture2DAsset> sourceImage)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
		CubemapImageGen imageSrc = {};
		imageSrc.pSourceImage = sourceImage->GetSourceImageAsset()->Value.As<Image2D>();

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap " + imageSrc.pSourceImage->GetDebugName();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Width = 2048;
		radianceInfo.Height = 2048;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.pImageSrc = &imageSrc;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);

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

		PipelineSpecification irradiancePipelineInfo = {};
		irradiancePipelineInfo.DebugName = "EnvironmentIrradiance";
		irradiancePipelineInfo.ShaderPath = "res/Shaders/Compute/EnvironmentIrradiance.glsl";
		irradiancePipelineInfo.Usage = PipelineUsage::ComputeBit;

		Ref<Pipeline> irradiancePipeline = Pipeline::Create(&irradiancePipelineInfo);
		irradiancePipeline->GetShader()->Set("u_RadianceMap", 0, radianceMap->Value.As<CubemapTexture>());

		CubemapGen irradianceSource = {};
		irradianceSource.OutputImageName = "o_IrradianceMap";
		irradianceSource.pCubemap = radianceMap->Value.As<CubemapTexture>();
		irradianceSource.Pipeline = irradiancePipeline;

		CubemapTextureCreateInfo irradianceInfo = {};
		irradianceInfo.DebugName = "IrradianceMap";
		irradianceInfo.Width = 32;
		irradianceInfo.Height = 32;
		irradianceInfo.Usage = ImageUsage::Texture;
		irradianceInfo.pCubemapSrc = &irradianceSource;
		irradianceInfo.Format = ImageFormat::RGBA;
		irradianceInfo.GenerateMips = true;

		Ref<CubemapTexture> irradianceMap = CubemapTexture::Create(&irradianceInfo);

		IrradianceMap = AssetPointer::Create(irradianceMap, AssetType::EnvironmentMap);

		AssetMetadata irradianceMetadata = {};
		irradianceMetadata.Handle = IrradianceMap->GetHandle();
		irradianceMetadata.Type = AssetType::EnvironmentMap;
		AssetManager::AddRuntimeAsset(irradianceMetadata, IrradianceMap);
	}
	void EnvironmentMap::GeneratePreFilter(Ref<AssetPointer> radiance)
	{
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
	}
	Ref<EnvironmentMap> EnvironmentMap::Create(Ref<Texture2DAsset> sourceImage)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;
		Timer timer;

		Ref<EnvironmentMap> map = Ref<EnvironmentMap>::Create();
		map->GenerateRadiance(sourceImage);
		map->GenerateIrradiance(map->RadianceMap);
		//map->GeneratePreFilter(map->IrradianceMap);

		HZR_CORE_WARN("Environment map took {0} ms to load", timer.ElapsedMillis());
		return map;
	}
}