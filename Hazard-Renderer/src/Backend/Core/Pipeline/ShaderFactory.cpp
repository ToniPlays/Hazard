
#include "ShaderFactory.h"
#include "Utility/StringUtil.h"

#include "Shader.h"
#if HZR_PLATFORM_WINDOWS

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "spdlog/fmt/fmt.h"

#include "CachedBuffer.h"

namespace HazardRenderer
{
	shaderc::Compiler compiler;

	static std::string GetShaderStageCache(ShaderStage type) {
		switch (type)
		{
		case ShaderStage::Vertex:	return "vert";
		case ShaderStage::Fragment:	return "frag";
		case ShaderStage::Compute:	return "comp";
		case ShaderStage::Geometry:	return "geom";
		}
		HZR_ASSERT(false, "");
		return "";
	}
	static std::string GetRendererCache(RenderAPI api) {
		switch (api)
		{
		case RenderAPI::OpenGL:		return "gl";
		case RenderAPI::Vulkan:		return "vk";
		case RenderAPI::DX11:		return "dx11";
		case RenderAPI::DX12:		return "dx12";
		case RenderAPI::Metal:		return "met";
		}
		HZR_ASSERT(false, "");
		return "";
	}
	CacheStatus ShaderFactory::HasCachedShader(const std::filesystem::path& path, RenderAPI api)
	{
		auto cachePath = GetCachedFilePath(path, api);
		if (File::IsNewerThan(path, cachePath)) 
			return CacheStatus::Outdated;

		return File::Exists(cachePath) ? CacheStatus::Exists : CacheStatus::None;
	}

	bool ShaderFactory::CacheShader(const std::filesystem::path& path, const std::unordered_map<ShaderStage, Buffer> binaries, RenderAPI api)
	{
		//Determine cache size
		size_t size = 0;
		for (auto& [stage, shaderCode] : binaries)
		{
			size += sizeof(ShaderCacheData);
			size += shaderCode.Size;
		}

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

	std::unordered_map<ShaderStage, std::string> ShaderFactory::GetShaderSources(const std::filesystem::path& path)
	{
		HZR_ASSERT(File::Exists(path), "Shader source file does not exist");
		std::string sourceFile = File::ReadFile(path);
		std::unordered_map<ShaderStage, std::string> result;

		const char* typeToken = "#type";
		size_t endPos = 0;

		while (endPos != std::string::npos)
		{
			std::string type = StringUtil::GetPreprocessor(typeToken, sourceFile, endPos, &endPos);
			if (endPos == std::string::npos) continue;

			size_t nextTokenPos = sourceFile.find(typeToken, endPos);
			std::string src = nextTokenPos == std::string::npos ? sourceFile.substr(endPos) : sourceFile.substr(endPos, nextTokenPos - endPos);

			if (!PreprocessSource(path, src)) continue;
			result[Utils::ShaderStageFromString(type)] = src;
		}

		return result;
	}
	std::unordered_map<ShaderStage, Buffer> ShaderFactory::GetShaderBinaries(const std::filesystem::path& path, RenderAPI api)
	{
		CachedBuffer buffer = File::ReadBinaryFile(GetCachedFilePath(path, api));
		std::unordered_map<ShaderStage, Buffer> result;

		while (buffer.Available())
		{
			ShaderCacheData data = buffer.Read<ShaderCacheData>();
			result[data.ShaderStage] = Buffer::Copy(buffer.Read<Buffer>(data.Length));
		}
		return result;
	}
	bool ShaderFactory::PreprocessSource(const std::filesystem::path& path, std::string& shaderSource)
	{
		return PreprocessIncludes(path, shaderSource);
	}
	bool ShaderFactory::PreprocessIncludes(const std::filesystem::path& path, std::string& source)
	{
		std::string token = "#include";
		size_t offset = 0;
		
		while (offset != std::string::npos)
		{
			std::string value = StringUtil::GetPreprocessor(token.c_str(), source, offset, &offset);
			if (offset == std::string::npos) continue;
			std::string_view includePath = StringUtil::Between(value, "\"", "\"");
			std::string line = token + " " + value;
			std::filesystem::path inclPath = path.parent_path() / includePath;
			if (!File::Exists(inclPath))
			{
				Window::SendDebugMessage({ 
					Severity::Warning, 
					fmt::format("Preprocessor {0} failed", token), 
					fmt::format("Include: {0}\nIn shader: {1}", value, path.string())});

				return false;
			}
			
			source = StringUtil::Replace(source, line, File::ReadFile(path.parent_path() / includePath));
		}
		return true;
	}
	std::filesystem::path ShaderFactory::GetCachedFilePath(const std::filesystem::path& path, RenderAPI api)
	{
		std::string name = File::GetNameNoExt(path);
		std::string extension = GetRendererCache(api);
		return s_CacheDir / (name + "." + extension + ".hzrche");

	}
}
#endif
