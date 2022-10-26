#include <hzrpch.h>
#include "EnvironmentMap.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/RenderContext/TextureFactory.h"

namespace Hazard
{
	Ref<EnvironmentMap> EnvironmentMap::Create(Ref<Texture2DAsset> sourceImage)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;
		using namespace HazardRenderer;

		HZR_CORE_INFO("Somehow generate the cubemap for environment map");

		CubemapImageGen imageSrc = {};
		imageSrc.pSourceImage = sourceImage->GetSourceImageAsset()->Value.As<Image2D>();

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap";
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Width = 2048;
		radianceInfo.Height = 2048;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.pImageSrc = &imageSrc;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);

		Ref<AssetPointer> radianceAsset = AssetPointer::Create(radianceMap, AssetType::EnvironmentMap);
		AssetMetadata radianceMetadata = {};
		radianceMetadata.Handle = radianceAsset->GetHandle();
		radianceMetadata.Type = AssetType::EnvironmentMap;

		AssetManager::AddRuntimeAsset(radianceMetadata, radianceAsset);

		//Irradiance map generation
		PipelineSpecification irradiancePipelineInfo = {};
		irradiancePipelineInfo.DebugName = "EnvironmentIrradiance";
		irradiancePipelineInfo.ShaderPath = "res/Shaders/Compute/EnvironmentIrradiance.glsl";
		irradiancePipelineInfo.Usage = PipelineUsage::ComputeBit;

		Ref<Pipeline> irradiancePipeline = Pipeline::Create(&irradiancePipelineInfo);
		irradiancePipeline->GetShader()->Set("u_RadianceMap", 0, radianceMap);

		CubemapGen irradianceSource = {};
		irradianceSource.OutputImageName = "o_IrradianceMap";
		irradianceSource.pCubemap = radianceMap;
		irradianceSource.Pipeline = irradiancePipeline;

		CubemapTextureCreateInfo irradianceInfo = {};
		irradianceInfo.Usage = ImageUsage::Texture;
		irradianceInfo.pCubemapSrc = &irradianceSource;
		irradianceInfo.Format = ImageFormat::RGBA;

		Ref<CubemapTexture> irradianceMap = CubemapTexture::Create(&irradianceInfo);

		Ref<AssetPointer> irradianceAsset = AssetPointer::Create(irradianceMap, AssetType::EnvironmentMap);

		AssetMetadata irradianceMetadata = {};
		irradianceMetadata.Handle = irradianceAsset->GetHandle();
		irradianceMetadata.Type = AssetType::EnvironmentMap;
		AssetManager::AddRuntimeAsset(irradianceMetadata, irradianceAsset);

		//Generate prefilter map
		PipelineSpecification preFilterPipelineInfo = {};
		preFilterPipelineInfo.DebugName = "EnvironmentMipFilter";
		preFilterPipelineInfo.ShaderPath = "res/Shaders/Compute/EnvironmentMipFilter.glsl";
		preFilterPipelineInfo.Usage = PipelineUsage::ComputeBit;

		Ref<Pipeline> preFilterPipeline = Pipeline::Create(&preFilterPipelineInfo);
		preFilterPipeline->GetShader()->Set("u_InputCube", 0, radianceMap);

		CubemapGen preFilterSource = {};
		preFilterSource.OutputImageName = "o_Texture";
		preFilterSource.pCubemap = radianceMap;
		preFilterSource.Pipeline = preFilterPipeline;

		CubemapTextureCreateInfo prefilterMapInfo = {};
		prefilterMapInfo.Usage = ImageUsage::Texture;
		prefilterMapInfo.pCubemapSrc = &preFilterSource;
		prefilterMapInfo.Format = ImageFormat::RGBA;
		Ref<CubemapTexture> preFilterMap = CubemapTexture::Create(&prefilterMapInfo);

		Ref<AssetPointer> preFilterMapAsset = AssetPointer::Create(preFilterMap, AssetType::EnvironmentMap);

		AssetMetadata prefilterMetadata = {};
		prefilterMetadata.Handle = preFilterMapAsset->GetHandle();
		prefilterMetadata.Type = AssetType::EnvironmentMap;
		AssetManager::AddRuntimeAsset(prefilterMetadata, preFilterMapAsset);

		AssetHandle brdfHandle = AssetManager::GetHandleFromFile("res/Textures/BRDF_LUT.tga");
		TextureHeader header = TextureFactory::GetFromCacheOrReload(brdfHandle, "res/Textures/BRDF_LUT.tga");

		Image2DCreateInfo brdfLutInfo = {};
		brdfLutInfo.DebugName = "BRDFLutTexture";
		brdfLutInfo.Width = header.Width;
		brdfLutInfo.Height = header.Height;
		brdfLutInfo.Data = header.ImageData;
		brdfLutInfo.Usage = ImageUsage::Attachment;

		Ref<Image2D> BRDFLut = Image2D::Create(&brdfLutInfo);

		Ref<AssetPointer> BRDFLutAsset = AssetPointer::Create(BRDFLut, AssetType::Image);

		AssetMetadata BRDLutMetadata = {};
		BRDLutMetadata.Handle = BRDFLutAsset->GetHandle();
		BRDLutMetadata.Type = AssetType::Image;
		AssetManager::AddRuntimeAsset(BRDLutMetadata, BRDFLutAsset);

		Ref<EnvironmentMap> map = Ref<EnvironmentMap>::Create();
		map->SourceImage = sourceImage;
		map->RadianceMap = radianceAsset;
		map->IrradianceMap = irradianceAsset;
		map->PreFilterMap = preFilterMapAsset;
		map->BRDFLut = BRDFLutAsset;

		HZR_CORE_WARN("Environment map took {0} ms to load", timer.ElapsedMillis());
		return map;
	}
}