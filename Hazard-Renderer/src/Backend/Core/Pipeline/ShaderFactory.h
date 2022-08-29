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
    enum class ShaderStage : uint32_t;

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
        bool CacheResult = true;
        ShaderStage Stage;
        std::string Source;
        std::string Name;
        //Returned by compilation if succeeded
        std::vector<ShaderDefine> Defines;
    };

	class ShaderFactory
	{
	public:
        static void SetCacheLocation(const std::filesystem::path& path) {};
        static bool HasCachedShader(const ShaderStage& stage, const std::filesystem::path& path);
        static std::unordered_map<ShaderStage, std::string> GetShaderSources(const std::filesystem::path& path);

    private:
        static std::filesystem::path GetCachedFilePath(const ShaderStage& stage, const std::filesystem::path& path);

        static bool PreprocessSource(const std::filesystem::path& path, std::string& shaderSource);
        static bool PreprocessIncludes(const std::filesystem::path& path, std::string& source);

	private:
		static inline std::filesystem::path s_CacheDir = "Library/Shaders";
	};
}
#endif
