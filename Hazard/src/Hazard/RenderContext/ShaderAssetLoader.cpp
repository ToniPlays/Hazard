
#include <hzrpch.h>
#include "ShaderAssetLoader.h"
#include "Backend/Core/ShaderCompiler.h"
#include <Hazard/RenderContext/ShaderAsset.h>

#include "Backend/Core/GraphicsContext.h"
#include "Hazard/Assets/AssetPack.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Core/Application.h"

namespace Hazard
{
	Ref<JobGraph> ShaderAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		return nullptr;
		using namespace HazardRenderer;
		RenderAPI api = GraphicsContext::GetRenderAPI();
		Ref<ShaderAsset> shaderAsset = Ref<ShaderAsset>::Create();

		auto cachePath = ShaderCompiler::GetCachedFilePath(metadata.Path, api);
		if (File::Exists(cachePath))
		{
			CachedBuffer buffer = File::ReadBinaryFile(cachePath);
			AssetPackElement element = buffer.Read<AssetPackElement>();

			while (buffer.Available())
			{
				ShaderCode code = buffer.Read<ShaderCode>();
				Buffer shaderCode = buffer.Read<Buffer>(code.Length);

				shaderAsset->ShaderCode.push_back({ code.Stage, Buffer::Copy(shaderCode) });
			}

			shaderAsset->m_Handle = element.Handle;
			shaderAsset->m_Type = (AssetType)element.Type;
			asset = shaderAsset;
			return nullptr;
		}
		return nullptr;
	}
	Ref<JobGraph> ShaderAssetLoader::Save(Ref<Asset>& asset)
	{
		return nullptr;
	}
}
