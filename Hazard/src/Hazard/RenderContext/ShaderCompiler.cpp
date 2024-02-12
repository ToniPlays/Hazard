#include "ShaderCompiler.h"

#ifdef HZR_SHADER_COMPILER

#include "MathCore.h"
#include "Utility/StringUtil.h"

#include "OpenGL/OpenGLShaderCompiler.h"
#include "Vulkan/VulkanShaderCompiler.h"
#include "Metal/MetalShaderCompiler.h"

#include "spdlog/fmt/fmt.h"

namespace Hazard
{
	using namespace HazardRenderer;

	std::string ShaderCompiler::GetShaderFromSource(uint32_t type, const std::string& source, RenderAPI api)
	{
		switch (api)
		{
			case RenderAPI::OpenGL:
			{
				OpenGL::OpenGLShaderCompiler compiler;

				double compilationTime = 0.0;
				std::vector<ShaderDefine> defines = { { "VULKAN_API" } };

				//Compile to Vulkan SPV for reflection
				CompileInfo compileInfoVulkan = {
					.Name = "Shader",
					.Renderer = RenderAPI::Vulkan,
					.Optimization = Optimization::None,
					.Stage = type,
					.Source = source,
					.DefineCount = defines.size(),
					.pDefines = defines.data()
				};

				if (!compiler.Compile(&compileInfoVulkan))
					throw std::exception(compiler.GetErrorMessage().c_str());

				std::vector<ShaderDefine> glDefines = { { "OPENGL_API" } };

				//Compile to Vulkan SPV for OpenGL source
				CompileInfo compileInfoVkToGL = {
					.Name = "VKShader",
					.Renderer = RenderAPI::Vulkan,
					.Optimization = Optimization::None,
					.Stage = type,
					.Source = source,
					.DefineCount = glDefines.size(),
					.pDefines = glDefines.data()
				};

				if (!compiler.Compile(&compileInfoVkToGL))
					throw std::exception(compiler.GetErrorMessage().c_str());

				compilationTime += compiler.GetCompileTime();

				//Get OpenGL shader source from Vulkan binaries
				std::string glSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
					throw std::exception(compiler.GetErrorMessage().c_str());

				return glSource;
			}
			case RenderAPI::Vulkan:
			{
				Vulkan::VulkanShaderCompiler compiler;
				std::vector<ShaderDefine> defines = { { "VULKAN_API" } };

				//Compile to Vulkan SPV
				CompileInfo compileInfo = {
					.Name = "VKShader",
					.Renderer = RenderAPI::Vulkan,
					.Optimization = Optimization::None,
					.Stage = type,
					.Source = source,
					.DefineCount = defines.size(),
					.pDefines = defines.data(),
				};

				if (!compiler.Compile(&compileInfo))
					throw std::exception(compiler.GetErrorMessage().c_str());

				return std::string((char*)compiler.GetCompiledBinary().Data, compiler.GetCompiledBinary().Size);
			}
			case RenderAPI::Metal:
			{
				Metal::MetalShaderCompiler compiler;
				std::vector<ShaderDefine> defines = { { "METAL_API" } };

				//Compile to Vulkan SPV, convert to MSL later
				CompileInfo compileInfo = {
					.Name  = "MSLShader",
					.Renderer = RenderAPI::Vulkan,
					.Optimization = Optimization::None,
					.Stage = type,
					.Source = source,
					.DefineCount = defines.size(),
					.pDefines = defines.data()
				};

				if (!compiler.Compile(&compileInfo))
					throw std::exception(compiler.GetErrorMessage().c_str());

				std::string mslSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), mslSource))
					throw std::exception(compiler.GetErrorMessage().c_str());

				return mslSource;
			}
			default: return "";
		}
	}

	std::unordered_map<uint32_t, std::string> ShaderCompiler::GetShaderSources(const std::filesystem::path& path)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(File::Exists(path), fmt::format("Shader source file does not exist {}", File::GetFileAbsolutePath(path).string()));

		std::string sourceFile = File::ReadFile(path);
		std::unordered_map<uint32_t, std::string> result;

		const char* typeToken = "#type";
		uint64_t endPos = 0;

		while (endPos != std::string::npos)
		{
			std::string type = StringUtil::GetPreprocessor(typeToken, sourceFile, endPos, &endPos);
			if (endPos == std::string::npos) continue;

			uint64_t nextTokenPos = sourceFile.find(typeToken, endPos);
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
		uint64_t offset = 0;

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
#endif
