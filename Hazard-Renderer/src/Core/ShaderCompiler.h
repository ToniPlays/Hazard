#pragma once

#include "Core/Rendering/Shader.h"
#include "Core/RenderContextCreateInfo.h"

namespace HazardRenderer
{
	enum class Optimization { None = 0, Memory, Performance };

	struct ShaderStageData;

	struct ShaderCode
	{
		uint32_t Stage;
		uint64_t Length;
	};

	struct ShaderDefine
	{
		std::string Name;
		std::string Value;

		ShaderDefine(std::string name) : Name(name), Value("") {};
		ShaderDefine(std::string name, std::string value) : Name(name), Value(value) {};
	};

	struct CompileInfo
	{
		RenderAPI Renderer;
		Optimization Optimization;
		uint32_t Stage;
		std::string Source;
		std::string Name;
		uint64_t DefineCount = 0;
		ShaderDefine* pDefines = nullptr;
	};

	class ShaderCompiler
	{
	public:
		static ShaderData GetShaderResources(const std::unordered_map<uint32_t, Buffer>& binaries);
		static std::vector<ShaderStageCode> GetShaderBinariesFromSource(const std::filesystem::path& sourceFile, const RenderAPI& api);
		static uint64_t GetBinaryLength(const std::vector<ShaderStageCode>& binaries);

	private:
		static std::unordered_map<uint32_t, std::string> GetShaderSources(const std::filesystem::path& path);
		static bool PreprocessSource(const std::filesystem::path& path, std::string& shaderSource);
		static bool PreprocessIncludes(const std::filesystem::path& path, std::string& source);
        
        static std::vector<ShaderMemberType> GetTypeMembers(spirv_cross::Compiler& compiler, spirv_cross::Resource resource);
	};
}
