#pragma once
#include "Core/Core.h"
#include "glad/glad.h"

namespace Hazard {
	class HAZARD_API Shader {
	public:
		Shader(const std::string& vertex, const std::string& fragment);
		~Shader();
		void Bind() const;
		void Unbind() const;
		GLint GetProgram() { return program; }
	private:
		GLint program = -1;
	};
}