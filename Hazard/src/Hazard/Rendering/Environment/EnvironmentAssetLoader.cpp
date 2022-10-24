
#include <hzrpch.h>
#include "EnvironmentAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "HazardRendererCore.h"
#include "EnvironmentMap.h"
#include "Hazard/RenderContext/TextureFactory.h"

namespace Hazard 
{
	bool EnvironmentAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		Timer timer;
		using namespace HazardRenderer;

		HZR_CORE_INFO("Somehow generate the cubemap for environment map");

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.FilePath = metadata.Path.string();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Width = 2048;
		radianceInfo.Height = 2048;
		radianceInfo.Format = ImageFormat::RGBA16F;
		radianceInfo.FlipOnLoad = false;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);
		if (radianceMap)
		{
			AssetMetadata radianceMetadata = {};
			radianceMetadata.Handle = radianceMap->GetHandle();
			radianceMetadata.Type = AssetType::EnvironmentMap;

			AssetManager::AddRuntimeAsset(radianceMetadata, radianceMap);

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
			irradianceInfo.Format = ImageFormat::RGBA16F;

			Ref<CubemapTexture> irradianceMap = CubemapTexture::Create(&irradianceInfo);

			AssetMetadata irradianceMetadata = {};
			irradianceMetadata.Handle = irradianceMap->GetHandle();
			irradianceMetadata.Type = AssetType::EnvironmentMap;
			AssetManager::AddRuntimeAsset(irradianceMetadata, irradianceMap);

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
			prefilterMapInfo.Format = ImageFormat::RGBA16F;
			Ref<CubemapTexture> preFilterMap = CubemapTexture::Create(&prefilterMapInfo);

			AssetMetadata prefilterMetadata = {};
			prefilterMetadata.Handle = preFilterMap->GetHandle();
			prefilterMetadata.Type = AssetType::EnvironmentMap;
			AssetManager::AddRuntimeAsset(prefilterMetadata, preFilterMap);

			TextureHeader header = TextureFactory::LoadTexture("res/Textures/BRDF_LUT.tga");

			Image2DCreateInfo brdfLutInfo = {};
			brdfLutInfo.DebugName = "BRDFLutTexture";
			brdfLutInfo.Width = header.Width;
			brdfLutInfo.Height = header.Height;
			brdfLutInfo.Data = header.ImageData;
			brdfLutInfo.Usage = ImageUsage::Attachment;

			Ref<Image2D> BRDFLut = Image2D::Create(&brdfLutInfo);

			asset = Ref<EnvironmentMap>::Create();
			asset.As<EnvironmentMap>()->RadianceMap = radianceMap;
			asset.As<EnvironmentMap>()->IrradianceMap = irradianceMap;
			asset.As<EnvironmentMap>()->PreFilterMap = preFilterMap;
			asset.As<EnvironmentMap>()->BRDFLut = BRDFLut;
		}
		HZR_CORE_WARN("Environment map took {0} ms to load", timer.ElapsedMillis());

		return asset;
	}
	bool EnvironmentAssetLoader::Save(Ref<Asset>& asset)
	{
		return false;
	}
}