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

	std::string ShaderCompiler::GetShaderFromSource(uint32_t type, const std::string& source, HazardRenderer::RenderAPI api)
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

	std::unordered_map<uint32_t, std::string> ShaderCompiler::SplitSource(const std::string& source)
	{
		std::unordered_map<uint32_t, std::string> result;

		const char* typeToken = "#type";
		uint64_t endPos = 0;

		while (endPos != std::string::npos)
		{
			std::string type = StringUtil::GetPreprocessor(typeToken, source, endPos, &endPos);
			if (endPos == std::string::npos) continue;

			uint64_t nextTokenPos = source.find(typeToken, endPos);
			std::string src = nextTokenPos == std::string::npos ? source.substr(endPos) : source.substr(endPos, nextTokenPos - endPos);

			result[Utils::ShaderStageFromString(type)] = src;
		}

		return result;
	}

	std::string ShaderCompiler::EmitGLSL(std::shared_ptr<Shading::ATLNode> root)
	{
		struct Attribute {
			std::string Qualifier;
			std::string Type;
			std::string Name;
			int Location;
		};

		struct StructDef {
			std::string Name;
			std::string Body;
		};

		std::ostringstream out;
		std::unordered_map<std::string, Attribute> inputAttrs;
		std::unordered_map<std::string, Attribute> outputAttrs;
		std::unordered_map<std::string, StructDef> structDefs;

		int inputLoc = 0;
		int outputLoc = 0;

		for (auto& [key, source] : root->Children)
		{
			uint32_t flags = ShaderStageFlagsFromString(key);
			if (flags == SHADER_STAGE_NONE) continue;

			std::ostringstream ss;

			std::string type = key;
			StringUtil::ToLower(type);

			std::string version = root->Children["Version"]->Value;
			ss << fmt::format("#type {}\n/// {} Shader ////\n#version {}\n\n", type, ShaderStageFlagsToString(flags), version);

			if (source->Children.count("Include"))
				EmitIncludes(ss, source->Children["Include"]);

			if (root->Children.count("Properties"))
				EmitProperties(ss, root->Children["Properties"], flags);

			if (root->Children.count("Constants"))
				EmitPushConstants(ss, root->Children["Constants"]);

			EmitScope(ss, source);

			out << ss.str();
		}

		return StringUtil::RemoveEmpty(out.str());
	}

	void ShaderCompiler::EmitProperties(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node, uint32_t stageFlags)
	{
		int binding = 0;
		for (auto& [name, prop] : node->Children)
		{
			std::regex reg(R"((\w+)\s*,\s*(\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+))");
			std::smatch matches;

			if (std::regex_match(prop->Value, matches, reg))
			{
				std::string displayName = matches[1].str();
				std::string type = matches[2].str();
				int set = std::stoi(matches[3].str());
				int binding = std::stoi(matches[4].str());
				int length = std::stoi(matches[5].str());

				if (type == "Sampler2D" && stageFlags & ~(SHADER_STAGE_FRAGMENT_BIT | SHADER_STAGE_COMPUTE_BIT))
					continue;

				// Here you could generate actual shader code or resource bindings
				out << fmt::format("layout(set = {}, binding = {}) uniform {} {}{};\n", set, binding, type, name, length > 1 ? fmt::format("[{}]", length) : "");
			}
			else
			{
				//out << "// Failed to parse property: " << prop->Value << "\n";
			}

		}
		out << "\n";
	}

	void ShaderCompiler::EmitPushConstants(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node)
	{
		// Collect push constant members first
		struct PushConstMember {
			std::string Name;
			std::string Type;
			int Length;
		};
		std::vector<PushConstMember> members;

		for (auto& [name, prop] : node->Children)
		{
			std::regex reg(R"((\w+)\s*,\s*(\w+)\s*)"); // name,type,length
			std::smatch matches;

			if (std::regex_match(prop->Value, matches, reg))
			{
				PushConstMember m;
				m.Name = matches[1].str();
				m.Type = matches[2].str();
				m.Length = 1;
				members.push_back(m);
			}
			else
			{
				out << "// Failed to parse push constant property: " << prop->Value << "\n";
			}
		}

		if (!members.empty())
		{
			out << "layout(push_constant) uniform PushConstants {\n";
			for (auto& m : members)
			{
				out << "    " << m.Type << " " << m.Name;
				if (m.Length > 1)
					out << "[" << m.Length << "]";
				out << ";\n";
			}
			out << "} Constant;\n\n"; // `PUSH` is the instance name
		}
	}

	void ShaderCompiler::EmitScope(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node)
	{
		// Inputs
		if (node->Children.count("Source"))
		{
			std::string glslCode = node->Children["Source"]->Value;
			out << fmt::format("{0}", glslCode);
		}
	}
	void ShaderCompiler::EmitIncludes(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node)
	{
		auto lines = StringUtil::SplitString(node->Value, ',');
		for (auto& path : lines)
		{
			std::string p = StringUtil::Replace(path, std::regex(R"(\t|\r|\n)"), "");
			if (p.empty()) continue;

			auto target = std::filesystem::weakly_canonical(m_Path.parent_path() / p);
			out << fmt::format("//Include file: {}\n{}\n//----------- End of include -----------\n\n", p, File::ReadFile(target));
		}

		out << "\n";
	}

	std::string ShaderCompiler::TransformGLSL(const std::string& source) {

		struct Attribute {
			std::string Qualifier;
			std::string Type;
			std::string Name;
			int Location;
		};

		std::ostringstream out;
		std::unordered_map<std::string, Attribute> inputAttrs;
		std::unordered_map<std::string, Attribute> outputAttrs;
		int inputLoc = 0;
		int outputLoc = 0;

		std::string transformed = source;

		// Regex to find struct declarations with inheritance
		std::regex structRegex(R"(struct\s+(\w+)\s*:\s*(VS_IN|VS_OUT|FS_IN|FS_OUT)\s*\{([^}]+)\};)");
		std::smatch match;
		std::string::const_iterator searchStart(transformed.cbegin());

		while (std::regex_search(searchStart, transformed.cend(), match, structRegex)) {
			std::string structName = match[1];
			std::string baseType = match[2];
			std::string membersBlock = match[3];

			std::istringstream memberStream(membersBlock);
			std::string line;

			while (std::getline(memberStream, line, ';')) {
				std::regex memberRegex(R"(\s*(?:(flat)\s+)?(\w+)\s+(\w+)\s*)");
				std::smatch memberMatch;
				if (std::regex_search(line, memberMatch, memberRegex)) {
					std::string qualifier = memberMatch[1].matched ? memberMatch[1].str() + " " : "";
					std::string type = memberMatch[2];
					std::string name = memberMatch[3];
					Attribute attr = { qualifier, type, name, 0 };

					if (baseType == "VS_IN" || baseType == "FS_IN") {
						attr.Location = inputLoc++;
						attr.Name = "i_" + attr.Name;
						inputAttrs[name] = attr;
					}
					else 
					{
						attr.Location = outputLoc++;
						outputAttrs[name] = attr;
						attr.Name = "o_" + attr.Name;
					}
				}
			}

			// Remove the struct declaration from source
			transformed.erase(match.position(0), match.length(0));
			searchStart = transformed.cbegin(); // restart search
		}

		// Emit input attributes
		for (auto& [name, attr] : inputAttrs)
			out << fmt::format("layout (location = {}) in {} {};\n", attr.Location, attr.Type, attr.Name);


		out << "\n";

		// Emit output attributes
		for (auto& [name, attr] : outputAttrs)
			out << fmt::format("layout (location = {}) out {}{} {};\n", attr.Location, attr.Qualifier, attr.Type, attr.Name);

		out << "\n";

		// Replace IN/OUT member usage
		for (auto& [name, attr] : inputAttrs)
			transformed = StringUtil::Replace(transformed, fmt::format("IN.{}", name), attr.Name);
		
		for (auto& [name, attr] : outputAttrs)
			transformed = StringUtil::Replace(transformed, fmt::format("OUT.{}", name), attr.Name);

		
		out << transformed;

		return StringUtil::RemoveEmpty(out.str());
	}

	std::string ShaderCompiler::PostProcessUniformStructs(const std::string& source)
	{
		return source;
		std::string result = source;

		size_t searchPos = 0;
		while (searchPos < result.size())
		{
			// Find the next struct definition
			size_t structPos = result.find("struct ", searchPos);
			if (structPos == std::string::npos) break;

			size_t nameStart = structPos + 7;
			size_t nameEnd = result.find_first_of(" {", nameStart);
			if (nameEnd == std::string::npos) break;

			std::string structName = result.substr(nameStart, nameEnd - nameStart);

			// Find the body of the struct
			size_t braceOpen = result.find('{', nameEnd);
			if (braceOpen == std::string::npos) break;

			int braceCount = 1;
			size_t p = braceOpen + 1;
			while (p < result.size() && braceCount > 0)
			{
				if (result[p] == '{') braceCount++;
				else if (result[p] == '}') braceCount--;
				p++;
			}
			if (braceCount != 0) break;

			std::string structBody = result.substr(braceOpen + 1, p - braceOpen - 2);

			// Try to find a uniform declaration of this struct
			std::string uniformSearch = "uniform " + structName;
			size_t uniformPos = result.find(uniformSearch, p);
			bool isPushConstant = false;

			// If not found after struct, check for push_constant
			if (uniformPos == std::string::npos)
			{
				uniformSearch = "layout(push_constant) uniform " + structName;
				uniformPos = result.find(uniformSearch, p);
				isPushConstant = (uniformPos != std::string::npos);
			}

			// If no usage found, skip this struct
			if (uniformPos == std::string::npos)
			{
				searchPos = p;
				continue;
			}

			// Find the uniform name after struct keyword
			size_t uniformNameStart = result.find_first_not_of(" \t\r\n", uniformPos + uniformSearch.length());
			size_t uniformNameEnd = result.find_first_of(" ;\n", uniformNameStart);
			std::string uniformName = (uniformNameStart != std::string::npos && uniformNameEnd != std::string::npos)
				? result.substr(uniformNameStart, uniformNameEnd - uniformNameStart)
				: structName;

			// Build replacement
			std::ostringstream replacement;
			if (isPushConstant)
				replacement << "layout(push_constant) uniform " << structName << " {\n";
			else
				replacement << "layout(set = 0, binding = 0) uniform " << structName << "Block {\n";

			std::istringstream bodyStream(structBody);
			std::string line;
			while (std::getline(bodyStream, line))
			{
				line.erase(0, line.find_first_not_of(" \t\r\n"));
				line.erase(line.find_last_not_of(" \t\r\n") + 1);
				if (!line.empty())
					replacement << "    " << line << ";\n";
			}

			replacement << "} " << uniformName << ";\n";

			// Replace the struct definition in the source
			result.replace(structPos, p - structPos, replacement.str());

			// Move searchPos forward
			searchPos = structPos + replacement.str().length();
		}

		return result;
	}


}
