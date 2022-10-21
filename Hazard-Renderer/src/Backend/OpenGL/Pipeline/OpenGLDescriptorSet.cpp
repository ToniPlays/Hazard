#include "OpenGLDescriptorSet.h"

#include "OpenGLBuffers.h"
#include "../Textures/OpenGLImage2D.h"

#include <glad/glad.h>


namespace HazardRenderer::OpenGL
{
	void OpenGLDescriptorSet::AddWriteDescriptor(OpenGLWriteDescriptor writeDescriptor)
	{
		m_WriteDescriptors[writeDescriptor.Binding] = writeDescriptor;
	}
	OpenGLWriteDescriptor& OpenGLDescriptorSet::GetWriteDescriptor(const std::string& name)
	{
		for (auto& [binding, descriptor] : m_WriteDescriptors)
		{
			if (descriptor.DebugName == name) return descriptor;
		}
	}
	void OpenGLDescriptorSet::BindResources(uint32_t programID)
	{
		for (auto& [binding, descriptor] : m_WriteDescriptors)
		{
			switch (descriptor.Type)
			{
			case GL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				glBindBufferBase(GL_UNIFORM_BUFFER, descriptor.ActualBinding, descriptor.BoundValue.at(0).As<OpenGLUniformBuffer>()->GetBufferID());
				break;
			case GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				for(auto& [binding, value] : descriptor.BoundValue)
					glBindTextureUnit(binding, value.As<OpenGLImage2D>()->GetID());

				break;
			}
		}
	}
}
