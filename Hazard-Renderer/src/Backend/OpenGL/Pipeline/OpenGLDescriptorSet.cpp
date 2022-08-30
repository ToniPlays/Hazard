#include "OpenGLDescriptorSet.h"

namespace HazardRenderer::OpenGL
{
	void OpenGLDescriptorSet::AddWriteDescriptor(OpenGLWriteDescriptor writeDescriptor)
	{
		m_WriteDescriptors[writeDescriptor.Binding] = writeDescriptor;
	}
}
