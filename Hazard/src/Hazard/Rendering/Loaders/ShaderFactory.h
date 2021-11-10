#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Rendering
{
	enum class Optimization { None = 0, Memory, Performance };
	enum class ShaderType;

	struct ShaderStageData;

	struct ShaderDefine {
		std::string Name;
		std::string Value;

		ShaderDefine(std::string name) : Name(name), Value("") {};
		ShaderDefine(std::string name, std::string value) : Name(name), Value(value) {};
	};

	struct CompileInfo 
	{
		RenderAPI Environment;
		Optimization Optimization;
		bool CacheResult = true;
		std::string Path = "";
		ShaderType Stage;
		std::string Source;
		std::string Error;
		//Returned by compilation if succeeded
		std::vector<uint32_t> Binary;
		std::vector<ShaderDefine> Defines;

		bool Succeeded() { return Error.empty(); }
	};

	enum class ShaderType;

	class ShaderFactory
	{
	public:
		static bool CacheExists(const std::string& path, ShaderType type, RenderAPI renderer);
		static bool SourceFileChanged(const std::string& path, ShaderType type, RenderAPI renderer);
		static std::filesystem::path GetShaderCacheFile(const std::string& path);
		static std::filesystem::path GetShaderSourcePath(const std::string& path);
		static std::unordered_map<ShaderType, std::string> GetShaderSources(const std::string& path);
		static std::unordered_map<ShaderType, std::string> SourcePreprocess(const std::string& source);
		static std::vector<uint32_t> GetShaderBinaries(const std::string& path, ShaderType type, RenderAPI renderer);
		static std::filesystem::path GetShaderBinaryCache(const std::string& path, ShaderType type, RenderAPI renderer);
		static void Compile(CompileInfo* compileInfo);
		static bool SaveShaderBinary(const std::filesystem::path& path, const std::vector<uint32_t>& binary);
		static ShaderStageData GetShaderResources(const std::vector<uint32_t>& binary);
	private:
		static inline std::string m_CacheDir = "library";
	};
}