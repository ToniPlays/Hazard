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
	void OpenGLDescriptorSet::BindResources()
	{
		for (auto& [binding, descriptor] : m_WriteDescriptors)
		{
			switch (descriptor.Type)
			{
			case GL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				glBindBufferBase(GL_UNIFORM_BUFFER, descriptor.ActualBinding, descriptor.BoundValue.As<OpenGLUniformBuffer>()->GetBufferID());
				break;
			case GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				glBindTextureUnit(descriptor.ActualBinding, descriptor.BoundValue.As<OpenGLImage2D>()->GetID());
				break;

			}
		}
	}
}
