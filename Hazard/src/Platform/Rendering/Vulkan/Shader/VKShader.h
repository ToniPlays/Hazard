#pragma once

#include "Hazard/Rendering/Shader/Shader.h"

namespace Hazard::Rendering::Vulkan {

	class VKShader : public Shader {
	public:
		VKShader(ShaderCreateInfo info);
		~VKShader();

		void Bind() const override;
		void Unbind() const override;

		ShaderInfo GetInfo() const { return m_Info; }

		void SetUniformInt(const std::string& name, int value) override;
		void SetUniformIntArray(const std::string& name, int* value, uint32_t size) override;
		void SetUniformFloat(const std::string& name, float value) override;
		void SetUniformVec2(const std::string& name, glm::vec2 value) override;
		void SetUniformVec3(const std::string& name, glm::vec3  value) override;
		void SetUniformMat4(const std::string& name, glm::mat4 value) override;
		void SetUniformColor(const std::string& name, Color color) override;
		void SetUniformBool(const std::string& name, bool value) override;

	private:
		ShaderInfo m_Info;
	};
}