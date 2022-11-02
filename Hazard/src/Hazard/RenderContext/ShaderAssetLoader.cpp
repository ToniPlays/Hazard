
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
	LoadType ShaderAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		RenderAPI api = GraphicsContext::GetRenderAPI();
		Ref<ShaderAsset> shaderAsset = Ref<ShaderAsset>::Create();

		auto& cachePath = ShaderCompiler::GetCachedFilePath(metadata.Path, api);
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
			return LoadType::Cache;
		}

		shaderAsset->ShaderCode = ShaderCompiler::GetShaderBinariesFromSource(metadata.Path, api);
		shaderAsset->m_Type = AssetType::Shader;

		asset = shaderAsset;
		return LoadType::Source;
	}
	bool ShaderAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();
		using namespace HazardRenderer;

		auto& metadata = AssetManager::GetMetadata(asset->GetHandle());

		for (uint32_t api = (uint32_t)RenderAPI::First; api <= (uint32_t)RenderAPI::Last; api++)
		{
			ThreadPool& threadPool = Application::Get().GetThreadPool();

			Thread& thread = threadPool.GetThread();
			thread.OnCompletionHandler([](const Thread& thread) {
				HZR_CORE_WARN("Thread finished in {0} ms", thread.GetExecutionTime());
				});

			thread.Dispatch([path = metadata.Path, api, handle = asset->GetHandle()]() {
				auto& binaries = ShaderCompiler::GetShaderBinariesFromSource(path, (RenderAPI)api);
				size_t assetSize = ShaderCompiler::GetBinaryLength(binaries);
				CachedBuffer dataBuffer(sizeof(AssetPackElement) + assetSize);

				AssetPackElement element = {};
				element.Type = (uint32_t)AssetType::Shader;
				element.Handle = handle;
				element.AssetDataSize = assetSize;

				dataBuffer.Write(element);

				for (auto& [shaderStage, binary] : binaries)
				{
					ShaderCode code = { shaderStage, binary.Size };

					dataBuffer.Write(code);
					dataBuffer.Write(binary.Data, binary.Size);
				}

				auto& cachePath = ShaderCompiler::GetCachedFilePath(path, (RenderAPI)api);

				if (!File::DirectoryExists(cachePath.parent_path()))
					File::CreateDir(cachePath.parent_path());

				File::WriteBinaryFile(cachePath, dataBuffer.GetData(), dataBuffer.GetSize());

				for (auto& code : binaries)
					code.ShaderCode.Release();

			});
		}
		return false;
	}
}