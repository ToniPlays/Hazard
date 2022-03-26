#pragma once

#include "Core/Core.h"
#include "Core/RenderContextCreateInfo.h"
#include "Core/Window.h"
#include "ShaderData.h"

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace HazardRenderer
{
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
		static inline std::string m_CacheDir = "library/Shaders";
	};
}
