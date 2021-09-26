#pragma once

#include "Hazard/Rendering/Shader.h"
#include "vulkan/vulkan.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanShader : public Shader {
	public:
		VulkanShader(const std::string& file);
		~VulkanShader();

		void Reload() override;
		void Bind() override;
	private:
		std::string m_Path;
	};
}

