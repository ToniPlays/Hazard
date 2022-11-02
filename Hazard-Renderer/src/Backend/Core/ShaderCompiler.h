#pragma once

#include "Pipeline/Shader.h"
#include "Backend/Core/RenderContextCreateInfo.h"

namespace HazardRenderer
{
	enum class Optimization { None = 0, Memory, Performance };

	struct ShaderStageData;
	enum class ShaderStage : uint32_t;

	struct ShaderCode
	{
		ShaderStage Stage;
		uint32_t Length;
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
		ShaderStage Stage;
		std::string Source;
		std::string Name;
		uint32_t DefineCount = 0;
		ShaderDefine* pDefines = nullptr;
	};

	class ShaderCompiler
	{
	public:
		static ShaderData GetShaderResources(const std::unordered_map<ShaderStage, Buffer>& binaries);
		static std::vector<ShaderStageCode> GetShaderBinariesFromSource(const std::filesystem::path& sourceFile, const RenderAPI& api);
		static size_t GetBinaryLength(const std::vector<ShaderStageCode>& binaries);
		static std::filesystem::path GetCachedFilePath(const std::filesystem::path& path, RenderAPI api);

	private:
		static std::unordered_map<ShaderStage, std::string> GetShaderSources(const std::filesystem::path& path);
		static bool PreprocessSource(const std::filesystem::path& path, std::string& shaderSource);
		static bool PreprocessIncludes(const std::filesystem::path& path, std::string& source);
	};
}