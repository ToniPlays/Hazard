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
					throw CompileException(compiler.GetErrorMessage());

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
					throw CompileException(compiler.GetErrorMessage());

				compilationTime += compiler.GetCompileTime();

				//Get OpenGL shader source from Vulkan binaries
				std::string glSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
					throw CompileException(compiler.GetErrorMessage());

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
					throw CompileException(compiler.GetErrorMessage());

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
					throw CompileException(compiler.GetErrorMessage());


				std::string mslSource;
				if (!compiler.Decompile(compiler.GetCompiledBinary(), mslSource))
					throw CompileException(compiler.GetErrorMessage());

				return mslSource;
			}
			default: return "";
		}
	#else
		return "UNSUPPORTED";
	#endif
	}

	std::unordered_map<uint32_t, std::string> ShaderCompiler::GetShaderSources(const std::filesystem::path& path)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(File::Exists(path), fmt::format("Shader source file does not exist {}", File::GetFileAbsolutePath(path).string()));

		std::string sourceFile = File::ReadFile(path);
		return GetShaders(sourceFile, path);
	}

	std::unordered_map<uint32_t, std::string> ShaderCompiler::GetShaders(const std::string& source, const std::filesystem::path& relativePath)
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

			if (!PreprocessSource(relativePath, src))
				continue;

			result[Utils::ShaderStageFromString(type)] = src;
		}

		return result;
	}

	ShaderParseFileResult ShaderCompiler::ParseShaderFile(const std::filesystem::path& path)
	{
		std::string source = File::ReadFile(path);
		if (source.empty())
			throw JobException(fmt::format("Unable to read file", path.string()));

		std::string name = StringUtil::GetPreprocessor("Shader", source, 0);
		name = VerifyEncapsulationWith(name, '"');

		source = StringUtil::Between(source, "{", "}");
		auto props = ParseShaderObject(source);

		auto parsedProperties = GetLayoutFromProperties(props["Properties"].Source);

		ShaderParseFileResult result = {
			.Name = name,
			.Language = props["Language"].Source,
			.Version = props["Version"].Source,
			.Type = props["Type"].Source,
			.Constants = {},
			.Layouts = GenerateDescriptorLayouts(parsedProperties),
		};


		for (auto& [name, prop] : props)
		{
			if (prop.Type != ShaderPropertyType::Shader) continue;

			auto source = GenerateShader(prop, parsedProperties);
			result.Shaders.push_back(fmt::format("#type {}\n#version {}\n{}", prop.Scope, result.Version, source));
		}

		const std::string states[] = { "Depth ", "DepthWrite " };
		for (auto& [name, prop] : props)
		{
			for (auto& state : states)
			{
				if (state != name) continue;
				result.PipelineState[state] = prop.Source;
				break;
			}
		}

		return result;
	}


	std::unordered_map<std::string, ShaderCompilerSourceScope> ShaderCompiler::ParseShaderObject(const std::string& source)
	{
		struct Property {
			const std::string Name;
			ShaderPropertyType Type;
			bool Required;
			std::string DefaultValue;
		};

		const std::vector<Property> properties = { { "Language", ShaderPropertyType::String, true },
												   { "Type", ShaderPropertyType::String, true },
												   { "Version", ShaderPropertyType::Number, true },
												   { "Properties", ShaderPropertyType::Object },
												   { "Vertex", ShaderPropertyType::Shader},
												   { "Fragment", ShaderPropertyType::Shader },
												   { "Compute", ShaderPropertyType::Shader },
												   { "Include", ShaderPropertyType::Object },
												   { "Shader", ShaderPropertyType::Object } ,
												   { "Depth ", ShaderPropertyType::String, false, "Less" },
												   { "DepthWrite ", ShaderPropertyType::Bool, false, "True" },
		};

		std::unordered_map<std::string, ShaderCompilerSourceScope> result;

		std::istringstream stream(source);
		std::string line;

		std::string currentScope;
		uint32_t scopeDepth = 0;

		while (std::getline(stream, line))
		{
			line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
			line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

			std::string srcLine = line;
			line.erase(0, line.find_first_not_of(' \t'));

			if (!currentScope.empty())
			{
				bool isScopeLine = false;
				if (line.find_first_of('{') != std::string::npos)
				{
					scopeDepth++;
					isScopeLine = scopeDepth == 1;
				}
				else if (line.find_first_of('}') != std::string::npos)
				{
					scopeDepth--;
					isScopeLine = scopeDepth == 0;
				}

				if (!isScopeLine)
					result[currentScope].Source += srcLine + "\n";

				if (scopeDepth == 0)
					currentScope.clear();

				continue;
			}

			for (auto& prop : properties)
			{
				if (!line.starts_with(prop.Name)) continue;

				switch (prop.Type)
				{
					case ShaderPropertyType::String:
					{
						std::string value = StringUtil::GetPreprocessor(prop.Name, line, 0);
						result[prop.Name] = { prop.Name, VerifyEncapsulationWith(value, '"'), prop.Type };
						break;
					}
					case ShaderPropertyType::Number:
					{
						try {
							std::string value = StringUtil::GetPreprocessor(prop.Name, line, 0);
							std::stoi(value);
							result[prop.Name] = { prop.Name, value, prop.Type };
							break;
						}
						catch (std::exception e)
						{
							throw JobException("String was not number");
						}
					}
					case ShaderPropertyType::Bool:
					{
						std::string value = StringUtil::GetPreprocessor(prop.Name, line, 0);
						result[prop.Name] = { prop.Name, value, prop.Type };
						break;
					}
					case ShaderPropertyType::Shader:
					case ShaderPropertyType::Object:
					{
						currentScope = prop.Name;

						result[prop.Name] = { currentScope, "", prop.Type };
						if (line.find_last_of("{") != std::string::npos)
							scopeDepth++;

						break;
					}
				}
			}
		}

		return result;
	}

	std::string ShaderCompiler::GenerateShader(const ShaderCompilerSourceScope& scope, std::vector<ShaderProperty>& layouts)
	{
		auto properties = ParseShaderObject(scope.Source);
		std::stringstream ss;

		std::istringstream stream(properties["Include"].Source);
		std::string line;

		while (std::getline(stream, line))
		{
			line.erase(0, line.find_first_not_of('\t'));
			ss << "#include " << line << "\n";
		}


		auto shaderBlocks = GetShaderPropertyBlocks(properties["Shader"].Source);

		ss << "{UNIFORM_BLOCK}" << "\n";
		ss << properties["Shader"].Source << "\n";

		std::string source = ss.str();

		//Preprocess structs
		for (auto& block : shaderBlocks)
		{
			if (block.Type.empty()) continue;
			source = StringUtil::Replace(source, block.Source, GeneratePropertyBlockSource(block));
		}


		//Handle structs
		for (auto& block : shaderBlocks)
		{
			if (block.Type.find("_IN") != std::string::npos)
			{
				for (auto& e : block.Elements)
					source = StringUtil::Replace(source, fmt::format("IN.{}", e.Name), fmt::format("IN_{}", e.Name));
			}
			else if (block.Type.find("_OUT") != std::string::npos)
			{
				for (auto& e : block.Elements)
					source = StringUtil::Replace(source, fmt::format("OUT.{}", e.Name), fmt::format("OUT_{}", e.Name));
			}
		}

		std::stringstream uniformStream;
		uniformStream << "\n";

		for (auto& uniform : layouts)
		{
			if (!StringUtil::Contains(source, uniform.Name)) continue;
			uniform.TypeFlags |= ShaderStageFlagsFromString(scope.Scope);

			std::string arr = "";
			if (uniform.Length > 1)
				arr = "[" + std::to_string(uniform.Length) + "]";

			std::string type = ShaderDataTypeToString(uniform.Type);
			if (uniform.Type == ShaderDataType::Other)
				type = GetShaderDescriptorType(uniform.TypeFlags);

			uniformStream << fmt::format("layout (set = {}, binding = {}) uniform {} {}{};", uniform.Set, uniform.Binding, type, uniform.Name, arr) << "\n";
		}
		
		return StringUtil::Replace(source, "{UNIFORM_BLOCK}", uniformStream.str());
	}

	std::vector<ShaderProperty> ShaderCompiler::GetLayoutFromProperties(const std::string& source)
	{
		std::vector<ShaderProperty> result;

		std::istringstream stream(source);
		std::string line;

		while (std::getline(stream, line))
		{
			line.erase(0, line.find_first_not_of(' \t'));
			auto& value = result.emplace_back();
			value.Name = line.substr(0, line.find_first_of(' '));

			auto data = StringUtil::Between(line, "(", ")");
			auto split = StringUtil::SplitString(data.data(), ',');

			for (auto& s : split)
				s.erase(0, s.find_first_not_of(' '));

			if (data.size() > 0)
				value.DisplayName = VerifyEncapsulationWith(split[0], '"');
			if (data.size() > 1)
			{
				value.Type = HazardRenderer::ShaderDataTypeFromString(split[1]);
				if (value.Type == ShaderDataType::Other)
					value.TypeFlags = HazardRenderer::GetShaderDescriptorType(split[1]);
			}
			if (data.size() > 2)
				value.Set = std::stoi(split[2]);
			if (data.size() > 3)
				value.Binding = std::stoi(split[3]);
			if (data.size() > 4)
				value.Length = std::stoi(split[4]);

			value.AccessFlags = SHADER_STAGE_ALL_GRAPHICS;
		}

		return result;
	}

	std::vector<HazardRenderer::DescriptorSetLayout> ShaderCompiler::GenerateDescriptorLayouts(const std::vector<ShaderProperty>& properties)
	{
		std::vector<HazardRenderer::DescriptorSetLayout> result;
		for (auto& prop : properties)
		{
			while (prop.Set > result.size())
				result.emplace_back();
		}

		for (auto& prop : properties)
		{
			DescriptorSetElement e = { prop.AccessFlags, prop.Name, prop.Binding, prop.Length, (DescriptorType)prop.TypeFlags };
			result[prop.Set - 1].GetElements().emplace_back(e);  //TODO: FIX -1
		}

		return result;
	}

	std::string ShaderCompiler::VerifyEncapsulationWith(const std::string& value, char c)
	{
		if (value.empty()) return "";

		uint64_t offset = 0;
		while (value[offset] == ' ')
			offset++;


		if (value[offset] != c)
			throw JobException(fmt::format("Unexpected '{0}', expected '{1}', parsing {2}", value[0], c, value));

		offset = value.find_first_of(c, offset + 1);

		if (offset == std::string::npos)
			throw JobException(fmt::format("Unexpected '{0}', expected '{1}', parsing {2}", value[value.length() - 1], c, value));

		std::string result = value.substr(value.find_first_of(c) + 1, offset - 1);

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

	std::vector<ShaderSourcePropertyBlock> ShaderCompiler::GetShaderPropertyBlocks(const std::string& shaderSource)
	{
		std::vector<ShaderSourcePropertyBlock> result;

		std::regex regex(R"((struct|union|class)\s+(\S+)(?:\s*:\s*(\S+))?\s*\{([^}]*)\}\s*;)");
		std::smatch match;

		ShaderSourcePropertyBlock current = {};
		std::string source = shaderSource;

		while (std::regex_search(source, match, regex))
		{
			auto& v = result.emplace_back();
			if (match[1].str() != "struct")
				throw JobException(fmt::format("Unexcepted type: {}", match[1].str()));
			v.Name = match[2].str();
			v.Type = match[3].str();
			v.Elements = ParseElements(match[4]);
			v.Source = match[0];
			source = match.suffix();
		}

		return result;
	}

	std::vector<ShaderPropertyElement> ShaderCompiler::ParseElements(const std::string& source)
	{
		std::vector<ShaderPropertyElement> result;

		auto lines = StringUtil::SplitString(source, ';');
		for (uint32_t i = 0; i < lines.size(); i++)
			lines[i] = std::regex_replace(lines[i], std::regex("[\t\r\n]"), "");

		std::stringstream ss;

		for (auto& line : lines)
		{
			if (line.empty()) continue;

			auto values = StringUtil::SplitString(line, ' ');

			if (values.size() < 2)
				throw JobException(fmt::format("Failed to parse line {}", line));

			if (values.size() == 2)
			{
				auto& v = result.emplace_back();
				v.Type = values[0];
				v.Name = values[1];
			}
			if (values.size() == 3)
			{
				auto& v = result.emplace_back();
				v.Prefix = values[0];
				v.Type = values[1];
				v.Name = values[2];
			}

			float i = 0;
		}

		return result;
	}

	std::string ShaderCompiler::GeneratePropertyBlockSource(const ShaderSourcePropertyBlock& block)
	{
		uint32_t location = 0;
		std::stringstream ss;

		std::string prefix = "";
		std::string type = "";

		if (block.Type.find("_IN") != std::string::npos)
		{
			prefix = "IN_";
			type = "in ";
		}
		else if (block.Type.find("_OUT") != std::string::npos)
		{
			prefix = "OUT_";
			type = "out ";
		}

		for (auto& e : block.Elements)
		{
			ss << fmt::format("layout(location = {0}) {1}{2}{3} {4}{5};\n\t\t\t", location, type, e.Prefix.empty() ? "" : e.Prefix + " ", e.Type, prefix, e.Name);
			location += ShaderDataTypeLocationSize(ShaderDataTypeFromString(e.Type));
		}

		return ss.str();
	}
}
