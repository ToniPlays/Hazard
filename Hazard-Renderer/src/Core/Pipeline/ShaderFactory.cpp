
#include "ShaderFactory.h"
#include "Utility/StringUtil.h"
#include "File/File.h"

#include "Shader.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

using namespace HazardUtility;

namespace HazardRenderer
{
	shaderc::Compiler compiler;

	static shaderc::CompileOptions GetCompileOptions(const CompileInfo& info) {
		shaderc::CompileOptions options;

		switch (info.Environment) {
		case RenderAPI::OpenGL:	options.SetTargetEnvironment(shaderc_target_env_opengl, 450); break;
		case RenderAPI::Vulkan:	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); break;
        default: break;
		}

		options.SetOptimizationLevel((shaderc_optimization_level)info.Optimization);

		for (auto& [Name, value] : info.Defines) {
			if (!value.empty())
				options.AddMacroDefinition(Name, value);
			else options.AddMacroDefinition(Name);
		}

		return options;
	}
	static shaderc_shader_kind ShaderStageToShaderC(ShaderType type) {
		switch (type)
		{
		case ShaderType::Vertex:	return shaderc_glsl_vertex_shader;
		case ShaderType::Fragment:	return shaderc_glsl_fragment_shader;
		case ShaderType::Compute:	return shaderc_glsl_compute_shader;
		case ShaderType::Geometry:	return shaderc_glsl_geometry_shader;
        case ShaderType::None:      return (shaderc_shader_kind)0;
		}
		return (shaderc_shader_kind)0;
	}

	static std::string GetShaderTypeCache(ShaderType type) {
		switch (type)
		{
		case ShaderType::Vertex:	return "vert";
		case ShaderType::Fragment:	return "frag";
		case ShaderType::Compute:	return "comp";
		case ShaderType::Geometry:	return "geom";
        case ShaderType::None:      return "non";
		}
		return "";
	}
	static std::string GetRendererCache(RenderAPI api) {
		switch (api)
		{
		case RenderAPI::OpenGL:		return "gl";
		case RenderAPI::Vulkan:		return "vulkan";
		case RenderAPI::DX11:		return "dx11";
		case RenderAPI::DX12:		return "dx12";
		case RenderAPI::Metal:		return "met";
        case RenderAPI::Auto:       return "";
		}
		return "";
	}

	bool ShaderFactory::CacheExists(const std::string& path, ShaderType type, RenderAPI renderer)
	{
		auto cacheDir = GetShaderCacheFile(GetShaderBinaryCache(path, type, renderer).string());
		return File::Exists(cacheDir);
	}
	bool ShaderFactory::SourceFileChanged(const std::string& path, ShaderType type, RenderAPI renderer)
	{
		auto sourceFile = GetShaderSourcePath(path);
		auto cacheFile = GetShaderCacheFile(GetShaderBinaryCache(path, type, renderer).string());

		if (!File::Exists(cacheFile))
			return false;

		return File::IsNewerThan(sourceFile, cacheFile);
	}
	std::filesystem::path ShaderFactory::GetShaderCacheFile(const std::string& path)
	{
		std::string fileName = File::GetName(path);
		return m_CacheDir + "/" + fileName;
	}
	std::filesystem::path ShaderFactory::GetShaderSourcePath(const std::string& path)
	{
		return "res/" + path;
	}
	std::unordered_map<ShaderType, std::string> ShaderFactory::GetShaderSources(const std::string& path)
	{
		std::string source = File::ReadFile(GetShaderSourcePath(path));
		return SourcePreprocess(source);
	}
	std::unordered_map<ShaderType, std::string> ShaderFactory::SourcePreprocess(const std::string& source)
	{
		std::unordered_map<ShaderType, std::string> shaderSources;

		const char* typeToken = "#type";
		StringUtil::PreprocessTypeSource(source, typeToken, [&](std::string type, std::string source) {
			shaderSources[Utils::ShaderTypeFromString(type)] = source;
			});

		return shaderSources;
	}
	std::vector<uint32_t> ShaderFactory::GetShaderBinaries(const std::string& path, ShaderType type, RenderAPI renderer)
	{
		std::vector<uint32_t> binaries;
		std::filesystem::path cachePath = GetShaderBinaryCache(GetShaderCacheFile(path).string(), type, renderer);

		if (SourceFileChanged(path, type, renderer)) {
			return binaries;
		}

		if (!File::Exists(cachePath))
			return binaries;

		File::ReadBinaryFileUint32(cachePath, binaries);
		return binaries;
	}
	std::filesystem::path ShaderFactory::GetShaderBinaryCache(const std::string& path, ShaderType type, RenderAPI renderer)
	{
		std::string fileName = File::GetPathNoExt(path);
		return fileName + "." + GetShaderTypeCache(type) + "." + GetRendererCache(renderer) + ".hzrche";
	}
	void ShaderFactory::Compile(CompileInfo* compileInfo)
	{
		if (compileInfo->Source.empty() && compileInfo->Binary.size() == 0) {
			compileInfo->Error = "Compilation failed: No source or binary provided";
			return;
		}


		if (compileInfo->Binary.size() != 0 && compileInfo->Source.empty()) {

			spirv_cross::CompilerGLSL glslCompiler(compileInfo->Binary);
			compileInfo->Source = glslCompiler.compile();
		}

		shaderc::CompileOptions options = GetCompileOptions(*compileInfo);
		options.SetGenerateDebugInfo();
		shaderc::CompilationResult result = compiler.CompileGlslToSpv(compileInfo->Source, ShaderStageToShaderC(compileInfo->Stage), compileInfo->Path.c_str(), options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			compileInfo->Error = result.GetErrorMessage();
			return;
		}

		compileInfo->Binary = std::vector<uint32_t>(result.begin(), result.end());

		if (compileInfo->CacheResult)
		{
			std::string cachePath = GetShaderBinaryCache(GetShaderCacheFile(compileInfo->Path).string(), compileInfo->Stage, compileInfo->Environment).string();
			SaveShaderBinary(cachePath, compileInfo->Binary);
		}
	}
	bool ShaderFactory::SaveShaderBinary(const std::filesystem::path& path, const std::vector<uint32_t>& binary)
	{
		if (binary.size() == 0) return false;

		if (!File::DirectoryExists(path.parent_path()))
			File::CreateDir(path.parent_path());

		return File::WriteBinaryFile(path, binary);
	}
	ShaderStageData ShaderFactory::GetShaderResources(const std::vector<uint32_t>& binary)
	{
		spirv_cross::Compiler compiler(binary);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		ShaderStageData shaderStage;

		for (auto& resource : resources.stage_inputs) {

			auto spvType = compiler.get_type(resource.base_type_id);
			ShaderStageInput input;
			input.Name = resource.name;
			input.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
			input.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
			input.Type = Utils::ShaderTypeFromSPV(spvType);
			input.Size = ShaderDataTypeSize(input.Type);

			shaderStage.Inputs[input.Location] = input;
			shaderStage.Stride += input.Size;
		}
		uint32_t offset = 0;

		for (uint32_t i = 0; i < shaderStage.Inputs.size(); i++)
		{
			shaderStage.Inputs[i].Offset = offset;
			offset += shaderStage.Inputs[i].Size;
		}

		for (auto& resource : resources.stage_outputs) {

			auto spvType = compiler.get_type(resource.base_type_id);
			ShaderStageOutput output;
			output.Name = resource.name;
			output.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
			output.Type = Utils::ShaderTypeFromSPV(spvType);
			output.Size = ShaderDataTypeSize(output.Type);

			shaderStage.Outputs[output.Location] = output;
		}
		for (auto& resource : resources.sampled_images) {

			auto spvType = compiler.get_type(resource.base_type_id);
			auto& type = compiler.get_type(resource.type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
			uint32_t arraySize = type.array[0] == 0 ? 1 : type.array[0];


			ShaderSampledImage output;
			output.Name = resource.name;
			output.Binding = binding;
			output.Dimension = spvType.image.dim;
			output.ArraySize = arraySize;

			shaderStage.SampledImages[binding] = output;
		}
		return shaderStage;
	}
}
