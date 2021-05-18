#pragma once

#include "Hazard/Rendering/Shader/Shader.h"

namespace Hazard::Rendering::Vulkan {
	class VulkanShader : public Shader {
	public:
		VulkanShader(const ShaderCreateInfo& info);
		~VulkanShader();
		void Bind() const;
		void Unbind() const;
		ShaderInfo GetInfo() const { return m_Info; };

	public:
		void SetUniformInt(const std::string& name, int value);
		void SetUniformIntArray(const std::string& name, int* value, uint32_t size);
		void SetUniformFloat(const std::string& name, float value);
		void SetUniformVec2(const std::string& name, glm::vec2 value);
		void SetUniformVec3(const std::string& name, glm::vec3  value);
		void SetUniformMat4(const std::string& name, glm::mat4 value);
		void SetUniformColor(const std::string& name, Color color);
		void SetUniformBool(const std::string& name, bool value);
		void SetUniformBlock(const std::string& name, void* value) {};
	private:
		ShaderInfo m_Info;
	};
}