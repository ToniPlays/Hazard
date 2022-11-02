
#include <hzrpch.h>
#include "PipelineAssetLoader.h"
#include "Hazard/Assets/AssetManager.h"
#include "Backend/Core/Pipeline/RenderPass.h"
#include "Backend/Core/Pipeline/ShaderFactory.h"

#include "Backend/Vulkan/VulkanShaderCompiler.h"

namespace Hazard
{
	using namespace HazardRenderer;

	LoadType PipelineAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		if (ShaderFactory::HasCachedShader(metadata.Path, RenderAPI::Vulkan) == CacheStatus::Exists)
		{
			CachedBuffer dataBuffer = File::ReadBinaryFile(ShaderFactory::GetCachedFilePath(metadata.Path, RenderAPI::Vulkan));

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

		Vulkan::VulkanShaderCompiler compiler;
		std::vector<ShaderStageCode> binaries;
		std::unordered_map<ShaderStage, std::string> sources = ShaderFactory::GetShaderSources(metadata.Path);

		//Compile Vulkan shader
		for (auto& [stage, source] : sources)
		{
			double compilationTime = 0.0;
			std::vector<ShaderDefine> defines = { { "VULKAN_API" } };
			//Compile to Vulkan SPV
			CompileInfo compileInfoVulkan = {};
			compileInfoVulkan.Renderer = RenderAPI::Vulkan;
			compileInfoVulkan.Name = metadata.Path.string();
			compileInfoVulkan.Stage = stage;
			compileInfoVulkan.Source = source;
			compileInfoVulkan.Defines = defines;

			if (!compiler.Compile(&compileInfoVulkan))
				continue;

			compilationTime += compiler.GetCompileTime();
			auto& bin = binaries.emplace_back();

			bin.Stage = stage;
			bin.ShaderCode = Buffer::Copy(compiler.GetCompiledBinary());
		}

		PipelineSpecification specs = {};
		specs.DebugName = metadata.Path.string();
		specs.pTargetRenderPass = nullptr;
		specs.ShaderPath = metadata.Path.string();
		specs.DrawType = DrawType::Fill;
		specs.Usage = PipelineUsage::GraphicsBit;
		specs.CullMode = CullMode::None;
		specs.DepthOperator = DepthOp::Less;
		specs.DepthTest = true;
		specs.DepthWrite = true;
		specs.UseShaderLayout = true;
		specs.ShaderCodeCount = binaries.size();
		specs.pShaderCode = binaries.data();

		asset = AssetPointer::Create(Pipeline::Create(&specs), AssetType::Pipeline);

		return LoadType::Source;
	}
	bool PipelineAssetLoader::Save(Ref<Asset>& asset)
	{
		HZR_PROFILE_FUNCTION();

		auto& metadata = AssetManager::GetMetadata(asset->GetHandle());
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

		for (uint32_t api = (uint32_t)RenderAPI::First; api < (uint32_t)RenderAPI::Last; api++)
		{
			auto& binaries = pipeline->GetShader()->GetShaderCode();
			CachedBuffer dataBuffer(sizeof(PipelineAssetHeader) + ShaderFactory::GetBinaryLength(binaries));

			dataBuffer.Write(header);

			for (auto& [shaderStage, binary] : binaries)
			{
				ShaderCode code = { shaderStage, binary.Size };

				dataBuffer.Write(code);
				dataBuffer.Write(binary.Data, binary.Size);
			}

			auto& path = ShaderFactory::GetCachedFilePath(metadata.Path, (RenderAPI)api);
			if (!File::DirectoryExists(path.parent_path()))
				File::CreateDir(path.parent_path());

			File::WriteBinaryFile(path, dataBuffer.GetData(), dataBuffer.GetSize());
		}

		return true;
	}
}