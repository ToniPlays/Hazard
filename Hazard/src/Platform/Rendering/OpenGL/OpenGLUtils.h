#pragma once

#include <glad/glad.h>
#include "Hazard/Rendering/Textures/Texture.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLUtils {
	public:
		static GLuint DataTypeToOpenGLType(TextureDataType type);
	};
}