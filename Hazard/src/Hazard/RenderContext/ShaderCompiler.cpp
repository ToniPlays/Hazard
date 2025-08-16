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

	std::string ShaderCompiler::GenerateGLSLFromBlock(uint32_t version, ParsedScope& block, ParsedScope& root)
	{
		std::string source, includes, uniforms;
		includes = block.GetValueOrDefault<std::string>("Include", "");
		uniforms = root.GetValueOrDefault<std::string>("Properties", "");
		block.RequireValue("Source", source);

		includes = StringUtil::Replace(includes, std::regex(R"(\t|\n|\r)"), "");
		uniforms = StringUtil::Replace(uniforms, std::regex(R"(\t|\n|\r)"), "");

		source = source.substr(StringUtil::OffsetOf(source, "{") + 1);

		if(includes.length() > 1)
			includes = includes.substr(1);

		std::string processedSource = ProcessSource(source);

		std::vector<std::string> paths = StringUtil::SplitString(includes, ',');
		std::string includeSource = "";

		for (auto& path : paths)
		{
			auto target = std::filesystem::weakly_canonical(m_Path.parent_path() / path);
			includeSource += fmt::format("//Include file: {}\n{}\n\n", target.string(), File::ReadFile(target));
		}

		std::vector<std::string> layouts = StringUtil::SplitString(uniforms, '\n');
		std::string uniformSource = "";

		return fmt::format("#version {}\n\n{}\n{}\n{}", version, includeSource, uniforms, processedSource);
	}

	std::string ShaderCompiler::ProcessSource(const std::string& source)
	{
		std::regex regex(R"(struct\s+(\w+)\s*:\s*(\w+)\s*\{([^}]*)\})");
		std::string processedSource = source;

		for (std::sregex_iterator it(source.begin(), source.end(), regex), end; it != end; ++it)
		{
			const std::smatch& m = *it;
			std::string type = m[2].str().substr(m[2].str().find_first_of('_') + 1);
			std::string src = "";
			std::string find = m[0].str();

			if (m[2] == "FS_OUT" || m[2] == "VS_IN")
			{
				uint32_t location = 0;
				for (auto& row : StringUtil::SplitString(m[3], ';'))
				{
					std::string data = StringUtil::Replace(row, std::regex(R"(^\s+|\s+\Z)"), "");
					if (data.length() == 0) continue;
					bool isOut = m[2] == "FS_OUT";

					std::string prefix = isOut ? "o" : "a";

					auto dataType = StringUtil::SplitString(data, ' ');
					src += fmt::format("layout (location = {0}) {1} {2} {3}_{4};\n", location, isOut ? "out" : "in", dataType[0], prefix, dataType[1]);
					location++;

					//Replace all OUT.X variables
					processedSource = StringUtil::Replace(processedSource, fmt::format("{0}.{1}", isOut ? "OUT" : "IN", dataType[1]), fmt::format("{}_{}", prefix, dataType[1]));
				}
				find += ";";
			}
			else if (m[2] == "VS_OUT" || m[2] == "FS_IN")
			{
				bool isOut = m[2] == "VS_OUT";
				std::string prefix = isOut ? "o" : "a";
				std::string struc = fmt::format("struct {0} {{\n{1}\n}};", m[1].str(), m[3].str());
				std::string replaceWith = fmt::format("{0}\nlayout(location = {1}) {2} {3} {4}", struc, 0, isOut ? "out" : "in", m[1].str(), isOut ? "OUT" : "IN");
				processedSource = StringUtil::Replace(processedSource, m[0].str(), replaceWith);
			}
			else src = fmt::format("struct {0} {{\n{1}\n}} {2}", m[1].str(), m[3].str(), type);

			processedSource = StringUtil::Replace(processedSource, find, src);
		}
		return StringUtil::Replace(processedSource, std::regex("\t"), "");
	}
}
