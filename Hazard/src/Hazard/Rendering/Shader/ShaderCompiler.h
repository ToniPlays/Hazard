#pragma once

namespace Hazard::Rendering 
{
	class ShaderCompiler {
	public:
		static bool CompileShader(const std::string& sourceFile, const std::string& dest);
	};
}