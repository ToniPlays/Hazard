
#include <hzrpch.h>
#include "Hazard/Core/Application.h"
#include "PipelineAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Backend/Core/Pipeline/RenderPass.h"

#include "Backend/Core/GraphicsContext.h"
#include "Backend/Core/ShaderCompiler.h"

namespace Hazard
{
	using namespace HazardRenderer;

	LoadType PipelineAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		RenderAPI currentAPI = GraphicsContext::GetRenderAPI();
		if (File::Exists(ShaderCompiler::GetCachedFilePath(metadata.Path, currentAPI)))
		{
			CachedBuffer dataBuffer = File::ReadBinaryFile(ShaderCompiler::GetCachedFilePath(metadata.Path, currentAPI));

			PipelineAssetHeader header = dataBuffer.Read<PipelineAssetHeader>();
			std::vector<ShaderStageCode> shaderCode(header.StageCount);

			for (uint32_t i = 0; i < header.StageCount; i++)
			{
				ShaderCode code = dataBuffer.Read<ShaderCode>();
				shaderCode[i].Stage = code.Stage;
				shaderCode[i].ShaderCode = Buffer::Copy(dataBuffer.Read<Buffer>(code.Length));
			}

			PipelineSpecification specs = {};
			specs.DebugName = metadata.Path.string();
			specs.pTargetRenderPass = nullptr;
			specs.ShaderPath = metadata.Path.string();
			specs.DrawType = header.DrawType;
			specs.Usage = header.Usage;
			specs.CullMode = header.CullMode;
			specs.DepthTest = header.DepthTest;
			specs.DepthWrite = header.DepthWrite;
			specs.UseShaderLayout = header.UseShaderLayout;
			specs.DepthOperator = header.DepthOperator;
			specs.ShaderCodeCount = shaderCode.size();
			specs.pShaderCode = shaderCode.data();

			asset = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);

			return LoadType::Cache;
		}

		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource(metadata.Path, currentAPI);

		PipelineSpecification specs = {};
		specs.DebugName = metadata.Path.string();
		specs.ShaderPath = metadata.Path.string();
		specs.DrawType = DrawType::Fill;
		specs.Usage = PipelineUsage::GraphicsBit;
		specs.CullMode = CullMode::None;
		specs.DepthOperator = DepthOp::Less;
		specs.DepthTest = true;
		specs.DepthWrite = true;
		specs.UseShaderLayout = true;
		specs.pTargetRenderPass = nullptr;
		specs.ShaderCodeCount = shaderCode.size();
		specs.pShaderCode = shaderCode.data();

		asset = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);

		for (auto& code : shaderCode)
			code.ShaderCode.Release();

		return LoadType::Source;
	}
	bool PipelineAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		auto& metadata = AssetManager::GetMetadata(asset->GetHandle());

		HZR_CORE_INFO("Saving asset {0}", metadata.Path.string());

		auto& pipeline = asset.As<AssetPointer>()->Value.As<Pipeline>();
		PipelineSpecification specs = pipeline->GetSpecifications();

		PipelineAssetHeader header = {};
		header.Usage = specs.Usage;
		header.DrawType = specs.DrawType;
		header.CullMode = specs.CullMode;
		header.LineWidth = specs.LineWidth;
		header.DepthTest = specs.DepthTest;
		header.DepthWrite = specs.DepthWrite;
		header.DepthOperator = specs.DepthOperator;
		header.UseShaderLayout = specs.UseShaderLayout;
		header.ElementCount = 0;
		header.StageCount = pipeline->GetShader()->GetShaderCode().size();

		std::vector<PipelineLayoutElement> layoutElements;

		if (!header.UseShaderLayout)
		{
			HZR_CORE_ASSERT(false, "Not implemented");

			header.ElementCount = specs.pBufferLayout->GetElementCount();
			layoutElements.reserve(header.ElementCount);

			for (auto& element : specs.pBufferLayout->GetElements())
			{
				auto& e = layoutElements.emplace_back();
				e.Type = element.Type;
				e.NameLength = element.Name.length();
				e.Name = element.Name.c_str();
			}
		}

		ThreadPool& threadPool = Application::Get().GetThreadPool();

		for (uint32_t api = (uint32_t)RenderAPI::First; api <= (uint32_t)RenderAPI::Last; api++)
		{
			Thread& thread = threadPool.GetThread();

			thread.OnCompletionHandler([](const Thread& thread) {
				HZR_CORE_WARN("Thread finished in {0} ms", thread.GetExecutionTime());
			});

			thread.Dispatch([header, path = metadata.Path, api]() {
				auto& binaries = ShaderCompiler::GetShaderBinariesFromSource(path, (RenderAPI)api);
				CachedBuffer dataBuffer(sizeof(PipelineAssetHeader) + ShaderCompiler::GetBinaryLength(binaries));

				dataBuffer.Write(header);

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

		return true;
	}
}