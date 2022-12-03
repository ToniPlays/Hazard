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
	OpenGLWriteDescriptor* OpenGLDescriptorSet::GetWriteDescriptor(const std::string& name)
	{
		for (auto& [binding, descriptor] : m_WriteDescriptors)
		{
			if (descriptor.DebugName == name) 
				return &descriptor;
		}
		return nullptr;
	}
	void OpenGLDescriptorSet::BindResources(uint32_t programID, bool isCompute)
	{
		HZR_PROFILE_FUNCTION();
		if (isCompute)
		{
			for (auto& [binding, descriptor] : m_WriteDescriptors)
			{
				switch (descriptor.Type)
				{
				case GL_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
					for (auto& [index, value] : descriptor.BoundValue)
					{
						if (!value) continue;
						uint32_t id = descriptor.Dimension == 3 ? value.As<OpenGLCubemapTexture>()->GetID() : value.As<OpenGLImage2D>()->GetID();
						glBindTextureUnit(descriptor.ActualBinding + index, id);
					}
					break;
				case GL_DESCRIPTOR_TYPE_STORAGE_IMAGE:
					for (auto& [index, value] : descriptor.BoundValue)
					{
						if (descriptor.Dimension == 3)
						{
							auto cubemap = value.As<OpenGLCubemapTexture>();
							uint32_t glFormat = OpenGLUtils::GetGLFormat(cubemap->GetFormat());
							glBindImageTexture(index, cubemap->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, glFormat);
						}
						else
						{
							auto image = value.As<OpenGLImage2D>();
							uint32_t glFormat = OpenGLUtils::GetGLFormat(image->GetFormat());
							glBindImageTexture(descriptor.ActualBinding + index, image->GetID(), 0, GL_TRUE, 0, GL_WRITE_ONLY, glFormat);
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
					for (auto& [index, value] : descriptor.BoundValue)
						glBindTextureUnit(descriptor.ActualBinding + index, value.As<OpenGLCubemapTexture>()->GetID());
				}
				else
				{
					for (auto& [index, value] : descriptor.BoundValue)
						glBindTextureUnit(descriptor.ActualBinding + index, value.As<OpenGLImage2D>()->GetID());
				}
				break;
			}
		}
	}
}
