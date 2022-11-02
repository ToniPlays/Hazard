

#if HZR_PLATFORM_WINDOWS

#include "Shader.h"
#include "Utility/StringUtil.h"
#include "Buffer/CachedBuffer.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "spdlog/fmt/fmt.h"

namespace HazardRenderer
{
	/*
	CacheStatus ShaderFactory::HasCachedShader(const std::filesystem::path& path, RenderAPI api)
	{
		HZR_PROFILE_FUNCTION();
		auto cachePath = GetCachedFilePath(path, api);
		if (File::IsNewerThan(path, cachePath)) 
			return CacheStatus::Outdated;

		return File::Exists(cachePath) ? CacheStatus::Exists : CacheStatus::None;
	}

	bool ShaderFactory::CacheShader(const std::filesystem::path& path, const std::unordered_map<ShaderStage, Buffer> binaries, RenderAPI api)
	{
		HZR_PROFILE_FUNCTION();
		//Determine cache size
		s

		CachedBuffer buffer(size);
		for (auto& [stage, shaderCode] : binaries)
		{
			ShaderCacheData data = {};
			data.ShaderStage = stage;
			data.Length = shaderCode.Size;

			buffer.Write(data);
			buffer.Write(shaderCode.Data, shaderCode.Size);
		}

		auto cachePath = GetCachedFilePath(path, api);

		if (!File::DirectoryExists(cachePath.parent_path()))
			File::CreateDir(cachePath.parent_path());

		return File::WriteBinaryFile(cachePath, buffer.GetData(), buffer.GetSize());
		
	}
	*/
}
#endif
