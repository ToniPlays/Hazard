#pragma once

#include "UtilityCore.h"
#include "Core/Core.h"

namespace HazardRenderer::OpenGL
{
	enum OpenGLDescriptorTypeFlags
	{
		GL_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		GL_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		GL_DESCRIPTOR_TYPE_MAX_ENUM,
		GL_DESCRIPTOR_TYPE_MAX_FLAGS
	};

	struct OpenGLWriteDescriptor 
	{
		std::string DebugName;
		OpenGLDescriptorTypeFlags Type = GL_DESCRIPTOR_TYPE_MAX_FLAGS;
		uint32_t Binding = UINT32_MAX;
		uint32_t ArraySize = 0;
		uint32_t Dimension = 1;
		uint32_t ActualBinding = UINT32_MAX;
		std::unordered_map<uint32_t, Ref<RefCount>> BoundValue;
	};

	class OpenGLDescriptorSet
	{
	public:
		OpenGLDescriptorSet() = default;
		~OpenGLDescriptorSet() = default;

		void AddWriteDescriptor(OpenGLWriteDescriptor writeDescriptor);
		std::unordered_map<uint32_t, OpenGLWriteDescriptor>& GetDescriptorSets() { return m_WriteDescriptors; }
		OpenGLWriteDescriptor* GetWriteDescriptor(uint32_t index) { return &m_WriteDescriptors[index]; }
		OpenGLWriteDescriptor* GetWriteDescriptor(const std::string& name);

		void BindResources(uint32_t programID, bool isCompute = false);

	private:
		std::unordered_map<uint32_t, OpenGLWriteDescriptor> m_WriteDescriptors;
	};
}