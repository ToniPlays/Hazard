#pragma once
#include "Core/Core.h"
#include "glad/glad.h"
#include "Shader.h"

namespace Hazard {
	class HAZARD_API OpenGLShader : public Shader {

	public:
		OpenGLShader(const std::string& vertex, const std::string& fragment);
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;
		uint32_t GetProgram() { return program; }
	};
}