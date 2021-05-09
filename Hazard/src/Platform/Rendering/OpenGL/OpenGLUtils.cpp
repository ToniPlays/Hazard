#pragma once

#include <hzrpch.h>
#include "OpenGLUtils.h"

namespace Hazard::Rendering::OpenGL 
{
	GLuint OpenGLUtils::DataTypeToOpenGLType(TextureDataType type)
	{
		switch (type)
		{
		case Hazard::Rendering::RGB:	return GL_RGB;
		case Hazard::Rendering::RGBA:	return GL_RGBA;
		case Hazard::Rendering::HDR:	return GL_RGB16F;
		case Hazard::Rendering::HDRA:	return GL_RGBA16F;
		}
	}
}