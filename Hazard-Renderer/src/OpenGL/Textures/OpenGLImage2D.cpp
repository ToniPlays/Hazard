#pragma once

#include "OpenGLImage2D.h"
#include "OpenGL/OpenGLUtils.h"

#include <glad/glad.h>

namespace HazardRenderer::OpenGL 
{
	OpenGLImage2D::OpenGLImage2D(Image2DCreateInfo* info)
 	{
		m_Width = info->Width;
		m_Height = info->Height;
		m_Format = info->Format;
		m_Samples = info->Mips;

		Invalidate();
	}
	OpenGLImage2D::~OpenGLImage2D()
	{
		Release();
	}
	void OpenGLImage2D::Invalidate()
	{
		if (m_ID)
		{
			Release();
		}

		bool multisampled = m_Samples > 1;
		OpenGLUtils::CreateTextures(multisampled, &m_ID, 1);
	}
	void OpenGLImage2D::Release()
	{
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
}
