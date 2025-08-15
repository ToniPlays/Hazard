#include <hzrpch.h>
#include "ShaderCompiler.h"

#include "Math/MathCore.h"
#include "CompileException.h"
#include "Utility/StringUtil.h"

#include "OpenGL/OpenGLShaderCompiler.h"
#include "Vulkan/VulkanShaderCompiler.h"
#include "Metal/MetalShaderCompiler.h"

#include "spdlog/fmt/fmt.h"
#include "Jobs/JobException.h"
#include <regex>


namespace Hazard
{
	using namespace HazardRenderer;

	std::string ShaderCompiler::GetShaderFromSource(uint32_t type, const std::string& source, RenderAPI api)
	{
	#ifdef SHADER_COMPILER
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
					throw CompileException(0, compiler.GetErrorMessage());

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
					throw CompileException(0, compiler.GetErrorMessage());

				compilationTime += compiler.GetCompileTime();

				//Get OpenGL shader source from Vulkan binaries
				std::string glSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
					throw CompileException(0, compiler.GetErrorMessage());

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
					throw CompileException(0, compiler.GetErrorMessage());

				return std::string((char*)compiler.GetCompiledBinary().Data, compiler.GetCompiledBinary().Size);
			}
			case RenderAPI::Metal:
			{
				Metal::MetalShaderCompiler compiler;
				std::vector<ShaderDefine> defines = { { "METAL_API" } };

				//Compile to Vulkan SPV, convert to MSL later
				CompileInfo compileInfo = {
					.Name = "VkToMSLShader",
					.Renderer = RenderAPI::Vulkan,
					.Optimization = Optimization::None,
					.Stage = type,
					.Source = source,
					.DefineCount = defines.size(),
					.pDefines = defines.data()
				};

				if (!compiler.Compile(&compileInfo))
					throw CompileException(0, compiler.GetErrorMessage());


				std::string mslSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), mslSource))
					throw CompileException(0, compiler.GetErrorMessage());

				return mslSource;
			}
			default:
				HZR_CORE_ASSERT(false, "Unknown RenderAPI");
				break;
		}
	#else
		return "UNSUPPORTED";
	#endif
	}

	std::string ShaderCompiler::GenerateGLSLFromBlock(uint32_t version, ParsedScope& block)
	{
		std::string source, includes;
		includes = block.GetValueOrDefault<std::string>("Include", "");
		block.RequireValue("Source", source);

		includes = StringUtil::Replace(includes, std::regex(R"(\t)"), "");
		includes = StringUtil::Replace(includes, std::regex(R"(\n)"), "");
		includes = StringUtil::Replace(includes, std::regex(R"(\r)"), "");
		source = source.substr(StringUtil::OffsetOf(source, "{") + 1);

		if(includes.length() > 1)
			includes = includes.substr(1);

		std::regex regex(R"(struct\s+(\w+)\s*:\s*(\w+)\s*\{([^}]*)\})");

		std::string processedSource = source;

		for (std::sregex_iterator it(source.begin(), source.end(), regex), end; it != end; ++it)
		{
			const std::smatch& m = *it;
			std::string type = m[2].str().substr(m[2].str().find_first_of('_') + 1);
			std::string src = "";
			std::string find = m[0].str();

			if(m[2] == "FS_OUT")
			{
				uint32_t location = 0;
				for (auto& row : StringUtil::SplitString(m[3], ';'))
				{
					std::string data = StringUtil::Replace(row, std::regex(R"(^\s+|\s+\Z)"), "");
					if (data.length() == 0) continue;
					

					auto dataType = StringUtil::SplitString(data, ' ');
					src += fmt::format("layout(location = {}) out {} o_{};\n", location, dataType[0], dataType[1]);
					location++;

					//Replace all OUT.X variables
					processedSource = StringUtil::Replace(processedSource, fmt::format("OUT.{}", dataType[1]), fmt::format("o_{}", dataType[1]));
				}
				find += ";";
			}
			else src = fmt::format("struct {0} {{\n{1}\n}} {2}", m[1].str(), m[3].str(), type);

			processedSource = StringUtil::Replace(processedSource, find, src);
		}

		std::vector<std::string> paths = StringUtil::SplitString(includes, ',');
		std::string includeSource = "";

		for (auto& path : paths)
		{
			auto target = std::filesystem::weakly_canonical(m_Path.parent_path() / path);
			includeSource += fmt::format("//Include file: {}\n{}\n\n", target.string(), File::ReadFile(target));
		}

		std::string result = fmt::format("#version {}\n\n{}\n{}", version, includeSource, processedSource);
		HZR_TRACE(processedSource);
		return result;
	}
}
