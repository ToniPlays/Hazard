#include "OpenGLDescriptorSet.h"

#include "OpenGLBuffers.h"
#include "../Textures/OpenGLImage2D.h"
#include "../Textures/OpenGLCubemapTexture.h"
#include "../OpenGLUtils.h"

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
			if (descriptor.DebugName == name) 
				return descriptor;
		}
		return OpenGLWriteDescriptor();
	}
	void OpenGLDescriptorSet::BindResources(uint32_t programID, bool isCompute)
	{
		if (isCompute)
		{
			for (auto& [binding, descriptor] : m_WriteDescriptors)
			{
				switch (descriptor.Type)
				{
				case GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					for (auto& [binding, value] : descriptor.BoundValue)
						glBindTextureUnit(descriptor.ActualBinding, value.As<OpenGLImage2D>()->GetID());
						//glBindImageTexture(descriptor.ActualBinding, image->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, glFormat);
					break;
				case GL_DESCRIPTOR_TYPE_STORAGE_IMAGE:
					for (auto& [binding, value] : descriptor.BoundValue)
					{
						if (descriptor.Dimension == 3)
						{
							auto& image = value.As<OpenGLCubemapTexture>();
							uint32_t glFormat = OpenGLUtils::GetGLFormat(image->GetFormat());
							glBindImageTexture(descriptor.ActualBinding, image->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, glFormat);
						}
						else
						{
							auto& image = value.As<OpenGLImage2D>();
							uint32_t glFormat = OpenGLUtils::GetGLFormat(image->GetFormat());
							glBindImageTexture(descriptor.ActualBinding, image->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, glFormat);
						}
					}
					break;
				}
			}
			return;
		}
		for (auto& [binding, descriptor] : m_WriteDescriptors)
		{
			switch (descriptor.Type)
			{
			case GL_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				glBindBufferBase(GL_UNIFORM_BUFFER, descriptor.ActualBinding, descriptor.BoundValue.at(0).As<OpenGLUniformBuffer>()->GetBufferID());
				break;
			case GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				if (descriptor.Dimension == 3)
				{
					for (auto& [binding, value] : descriptor.BoundValue)
						glBindTextureUnit(binding, value.As<OpenGLCubemapTexture>()->GetID());
					break;
				}
				for(auto& [binding, value] : descriptor.BoundValue)
					glBindTextureUnit(binding, value.As<OpenGLImage2D>()->GetID());

				break;
			}
		}
	}
}
