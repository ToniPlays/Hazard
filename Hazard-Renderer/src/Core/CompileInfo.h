#pragma once
#include "RenderContextCreateInfo.h"

namespace HazardRenderer
{
	enum class Optimization { None = 0, Memory, Performance };

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
}