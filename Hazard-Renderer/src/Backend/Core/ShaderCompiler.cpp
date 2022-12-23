#include "ShaderCompiler.h"

#include "MathCore.h"
#include "Utility/StringUtil.h"


#include "Backend/OpenGL/OpenGLShaderCompiler.h"
#include "Backend/Vulkan/VulkanShaderCompiler.h"

#include "spdlog/fmt/fmt.h"

namespace HazardRenderer
{
	static std::string GetShaderStageCache(ShaderStage type)
	{
		switch (type)
		{
		case ShaderStage::Vertex:	return "vert";
		case ShaderStage::Fragment:	return "frag";
		case ShaderStage::Compute:	return "comp";
		case ShaderStage::Geometry:	return "geom";
        default:
            HZR_ASSERT(false, "");
            break;
		}
		return "";
	}
	static std::string GetRendererCache(RenderAPI api)
	{
		switch (api)
		{
		case RenderAPI::OpenGL:		return "gl";
		case RenderAPI::Vulkan:		return "vk";
		case RenderAPI::DX11:		return "dx11";
		case RenderAPI::DX12:		return "dx12";
		case RenderAPI::Metal:		return "met";
		case RenderAPI::WebGL:		return "wgl";
        default:
            HZR_ASSERT(false, "");
            break;
		}
		
		return "";
	}


	ShaderData ShaderCompiler::GetShaderResources(const std::unordered_map<ShaderStage, Buffer>& binaries)
	{
		HZR_PROFILE_FUNCTION();
		ShaderData result;

		for (auto& [stage, binary] : binaries)
		{
			spirv_cross::Compiler compiler((uint32_t*)binary.Data, binary.Size / sizeof(uint32_t));
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			//Check stage inputs
			ShaderStageData& data = result.Stages[stage];
			for (auto& resource : resources.stage_inputs)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				uint32_t location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);

				ShaderStageInput& input = data.Inputs[location];
				input.Name = resource.name;
				input.Location = location;
				input.Type = Utils::ShaderDataTypeFromSPV(spvType);
				input.Size = ShaderDataTypeSize(input.Type);
				input.Offset = compiler.get_decoration(resource.id, spv::Decoration::DecorationOffset);
				data.Stride += input.Size;
			}
			for (auto& resource : resources.stage_outputs)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				uint32_t location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);

				ShaderStageOutput& output = data.Outputs[location];
				output.Name = resource.name;
				output.Location = location;
				output.Type = Utils::ShaderDataTypeFromSPV(spvType);
				output.Size = ShaderDataTypeSize(output.Type);
			}
			for (auto& resource : resources.acceleration_structures)
			{
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				auto& descriptorSet = result.AccelerationStructures[set];

				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				if (descriptorSet.find(binding) != descriptorSet.end())
				{
					auto& buffer = descriptorSet[binding];
					buffer.UsageFlags |= (uint32_t)stage;
				}
				else
				{
					auto& buffer = descriptorSet[binding];
					buffer.Name = resource.name;
					buffer.Location = binding;
					buffer.UsageFlags |= (uint32_t)stage;
				}
			}
			for (auto& resource : resources.storage_buffers)
			{
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				auto& descriptorSet = result.StorageBuffers[set];

				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				if (descriptorSet.find(binding) != descriptorSet.end())
				{
					auto& buffer = descriptorSet[binding];
					buffer.UsageFlags |= (uint32_t)stage;
				}
				else
				{
					auto& buffer = descriptorSet[binding];
					buffer.Name = resource.name;
					buffer.Location = binding;
					buffer.UsageFlags |= (uint32_t)stage;
				}
			}
			for (auto& resource : resources.uniform_buffers)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				auto& descriptorSet = result.UniformsDescriptions[set];

				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				if (descriptorSet.find(binding) != descriptorSet.end())
				{
					auto& buffer = descriptorSet[binding];
					buffer.UsageFlags |= (uint32_t)stage;
				}
				else
				{
					auto& buffer = descriptorSet[binding];
					buffer.Name = resource.name;
					buffer.Binding = binding;
					buffer.DescritorSet = set;
					buffer.MemberCount = spvType.member_types.size();
					buffer.Size = compiler.get_declared_struct_size(spvType);
					buffer.UsageFlags |= (uint32_t)stage;
				}
			}
			for (auto& resource : resources.sampled_images)
			{
				auto& type = compiler.get_type(resource.type_id);
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				auto& descriptorSet = result.ImageSamplers[set];

				auto& sampler = descriptorSet[binding];
				sampler.Name = resource.name;
				sampler.DescritorSet = set;
				sampler.Binding = binding;
				sampler.ArraySize = Math::Max<uint32_t>(type.array[0], 1);
				sampler.Dimension = type.image.dim == spv::DimCube ? 3 : 2;
				sampler.Flags |= (uint32_t)stage;
			}
			for (auto& resource : resources.storage_images)
			{
				auto& spvType = compiler.get_type(resource.base_type_id);
				auto& type = compiler.get_type(resource.type_id);
				uint32_t set = compiler.get_decoration(resource.id, spv::Decoration::DecorationDescriptorSet);
				uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);

				auto& descriptorSet = result.StorageImages[set];
				auto& storageImage = descriptorSet[binding];

				storageImage.Name = resource.name;
				storageImage.DescritorSet = set;
				storageImage.Binding = binding;
				storageImage.ArraySize = Math::Max<uint32_t>(type.array[0], 1);
				storageImage.Dimension = type.image.dim == spv::DimCube ? 3 : 2;
				storageImage.Flags |= (uint32_t)stage;

				switch (spvType.image.access)
				{
				case spv::AccessQualifier::AccessQualifierReadOnly: storageImage.Flags = ShaderResourceFlags_ReadOnly; break;
				case spv::AccessQualifier::AccessQualifierWriteOnly: storageImage.Flags = ShaderResourceFlags_WriteOnly; break;
				case spv::AccessQualifier::AccessQualifierReadWrite: storageImage.Flags = ShaderResourceFlags_ReadAndWrite; break;
                case spv::AccessQualifier::AccessQualifierMax: break;
                        
				}
				storageImage.Flags |= ShaderResourceFlags_WriteOnly;
			}

			//Sort input offsets
			uint32_t offset = 0;
			for (uint32_t i = 0; i < data.Inputs.size(); i++)
			{
				auto& input = data.Inputs[i];
				input.Offset = offset;
				offset += input.Size;
			}
		}
		return result;
	}
	std::vector<ShaderStageCode> ShaderCompiler::GetShaderBinariesFromSource(const std::filesystem::path& sourceFile, const RenderAPI& api)
	{
		Timer timer;
		auto sourceCode = GetShaderSources(sourceFile);

		std::vector<ShaderStageCode> result;

		switch (api)
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:
		{
			OpenGL::OpenGLShaderCompiler compiler;
			std::unordered_map<ShaderStage, Buffer> vulkanBinaries;

			for (auto& [stage, source] : sourceCode)
			{
				double compilationTime = 0.0;
				std::vector<ShaderDefine> defines = { { "VULKAN_API" } };

				//Compile to Vulkan SPV for reflection
				CompileInfo compileInfoVulkan = {};
				compileInfoVulkan.Renderer = RenderAPI::Vulkan;
				compileInfoVulkan.Name = File::GetName(sourceFile);
				compileInfoVulkan.Stage = stage;
				compileInfoVulkan.Source = source;
				compileInfoVulkan.DefineCount = defines.size();
				compileInfoVulkan.pDefines = defines.data();

				if (!compiler.Compile(&compileInfoVulkan))
				{
					std::cout << compiler.GetErrorMessage() << std::endl;
					continue;
				}

				compilationTime += compiler.GetCompileTime();
				vulkanBinaries[stage] = Buffer::Copy(compiler.GetCompiledBinary());

				std::vector<ShaderDefine> glDefines = { { "OPENGL_API" } };

				//Compile to Vulkan SPV for OpenGL source
				CompileInfo compileInfoVkToGL = {};
				compileInfoVkToGL.Renderer = RenderAPI::Vulkan;
				compileInfoVkToGL.Name = File::GetName(sourceFile);
				compileInfoVkToGL.Stage = stage;
				compileInfoVkToGL.Source = source;
				compileInfoVkToGL.DefineCount = glDefines.size();
				compileInfoVkToGL.pDefines = glDefines.data();

				if (!compiler.Compile(&compileInfoVkToGL))
				{
					std::cout << compiler.GetErrorMessage() << std::endl;
					continue;
				}

				compilationTime += compiler.GetCompileTime();

				//Get OpenGL shader source from Vulkan binaries
				std::string glSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
				{
					std::cout << compiler.GetErrorMessage() << std::endl;
					continue;
				}

				//Compile to OpenGL SPV
				CompileInfo compileInfoOpenGL = {};
				compileInfoOpenGL.Renderer = RenderAPI::OpenGL;
				compileInfoOpenGL.Name = File::GetName(sourceFile);
				compileInfoOpenGL.Stage = stage;
				compileInfoOpenGL.Source = glSource;
				compileInfoOpenGL.DefineCount = glDefines.size();
				compileInfoOpenGL.pDefines = glDefines.data();

				//Get OpenGL compiled binary
				if (!compiler.Compile(&compileInfoOpenGL))
				{
					std::cout << compiler.GetErrorMessage() << std::endl;
					continue;
				}

				result.push_back({ stage, Buffer::Copy(compiler.GetCompiledBinary()) });
			}
			break;
            
		}
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan:
		{
			Vulkan::VulkanShaderCompiler compiler;
			std::vector<ShaderDefine> defines = { { "VULKAN_API" } };

			for (auto& [stage, source] : sourceCode)
			{
				//Compile to Vulkan SPV
				CompileInfo compileInfoVulkan = {};
				compileInfoVulkan.Renderer = RenderAPI::Vulkan;
				compileInfoVulkan.Name = File::GetName(sourceFile);
				compileInfoVulkan.Stage = stage;
				compileInfoVulkan.Source = source;
				compileInfoVulkan.DefineCount = defines.size();
				compileInfoVulkan.pDefines = defines.data();

				if (!compiler.Compile(&compileInfoVulkan))
				{
					std::cout << fmt::format("Stage: {0} error: {1}", Utils::ShaderStageToString((uint32_t)stage), compiler.GetErrorMessage()) << std::endl;
					continue;
				}
				result.push_back({ stage, Buffer::Copy(compiler.GetCompiledBinary()) });
			}
			break;
		}
#endif
            default:
                break;
		}
		return result;
	}
	size_t ShaderCompiler::GetBinaryLength(const std::vector<ShaderStageCode>& binaries)
	{
		size_t size = 0;
		for (auto& [stage, shaderCode] : binaries)
		{
			size += sizeof(ShaderCode);
			size += shaderCode.Size;
		}
		return size;
	}

	std::filesystem::path ShaderCompiler::GetCachedFilePath(const std::filesystem::path& path, RenderAPI api)
	{
		HZR_PROFILE_FUNCTION();
		std::string name = File::GetNameNoExt(path);
		std::string extension = GetRendererCache(api);
		return s_CacheDir / (name + "." + extension + ".hzrche");
	}

	std::unordered_map<ShaderStage, std::string> ShaderCompiler::GetShaderSources(const std::filesystem::path& path)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(File::Exists(path), "Shader source file does not exist");
		std::string sourceFile = File::ReadFile(path);
		std::unordered_map<ShaderStage, std::string> result;

		const char* typeToken = "#type";
		size_t endPos = 0;

		while (endPos != std::string::npos)
		{
			std::string type = StringUtil::GetPreprocessor(typeToken, sourceFile, endPos, &endPos);
			if (endPos == std::string::npos) continue;

			size_t nextTokenPos = sourceFile.find(typeToken, endPos);
			std::string src = nextTokenPos == std::string::npos ? sourceFile.substr(endPos) : sourceFile.substr(endPos, nextTokenPos - endPos);

			if (!PreprocessSource(path, src)) 
				continue;

			result[Utils::ShaderStageFromString(type)] = src;
		}

		return result;
	}
	bool ShaderCompiler::PreprocessSource(const std::filesystem::path& path, std::string& shaderSource)
	{
		return PreprocessIncludes(path, shaderSource);
	}
	bool ShaderCompiler::PreprocessIncludes(const std::filesystem::path& path, std::string& source)
	{
		HZR_PROFILE_FUNCTION();
		std::string token = "#include";
		size_t offset = 0;

		bool success = true;

		while (offset != std::string::npos)
		{
			std::string value = StringUtil::GetPreprocessor(token.c_str(), source, offset, &offset);
			if (offset == std::string::npos) continue;
			std::string_view includePath = StringUtil::Between(value, "\"", "\"");
			std::string line = token + " " + value;
			std::filesystem::path inclPath = path.parent_path() / includePath;

			if (!File::Exists(inclPath))
			{
				std::cout << fmt::format("{2}: {0} could not open file {1}", token, inclPath.string(), path.string()) << std::endl;
				success = false;
				continue;
			}

			source = StringUtil::Replace(source, line, File::ReadFile(path.parent_path() / includePath));
		}
		return success;
	}
}
