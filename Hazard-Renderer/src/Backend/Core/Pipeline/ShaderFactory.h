#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_PLATFORM_WINDOWS

#include "Backend/Core/RenderContextCreateInfo.h"
#include "Backend/Core/Window.h"

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace HazardRenderer
{
    enum class Optimization { None = 0, Memory, Performance };

    struct ShaderStageData;
    enum class ShaderType;

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

	class ShaderFactory
	{
	public:

        static void SetCacheLocation(const std::filesystem::path& path) {};

		static std::unordered_map<ShaderType, std::string> GetShaderSources(const std::filesystem::path& path);
		static std::vector<uint32_t> GetShaderBinaries(const std::string& path, ShaderType type, RenderAPI renderer);
		static ShaderStageData GetShaderResources(const std::vector<uint32_t>& binary);
		static void Compile(CompileInfo* compileInfo);

    private:
		static bool CacheExists(const std::string& path, ShaderType type, RenderAPI renderer);
		static bool SourceFileChanged(const std::string& path, ShaderType type, RenderAPI renderer);
		static std::filesystem::path GetShaderCacheFile(const std::string& path);
		static std::filesystem::path GetShaderSourcePath(const std::string& path);
		static std::unordered_map<ShaderType, std::string> SourcePreprocess(const std::filesystem::path& relativePath, const std::string& source);
		static std::filesystem::path GetShaderBinaryCache(const std::string& path, ShaderType type, RenderAPI renderer);
		static bool SaveShaderBinary(const std::filesystem::path& path, const std::vector<uint32_t>& binary);

        static std::string PreprocessIncludes(const std::filesystem::path& relativePath, std::string& source);

	private:
		static inline std::string m_CacheDir = "library/Shaders";
	};
}
#endif
