#pragma once

#include "UtilityCore.h"

namespace HazardRenderer::OpenGL
{
	enum OpenGLDescriptorTypeFlags
	{
		GL_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		GL_DESCRIPTOR_TYPE_MAX_ENUM
	};

	struct OpenGLWriteDescriptor 
	{
		std::string DebugName;
		OpenGLDescriptorTypeFlags Type;
		uint32_t Binding = UINT32_MAX;
		uint32_t ArraySize = 0;
		uint32_t ActualBinding = UINT32_MAX;
	};

	class OpenGLDescriptorSet 
	{
	public:
		OpenGLDescriptorSet() = default;
		~OpenGLDescriptorSet() = default;

		void AddWriteDescriptor(OpenGLWriteDescriptor writeDescriptor);
		std::unordered_map<uint32_t, OpenGLWriteDescriptor>& GetDescriptorSets() { return m_WriteDescriptors; }

	private:
		std::unordered_map<uint32_t, OpenGLWriteDescriptor> m_WriteDescriptors;
	};
}