#pragma once

#include <glad/glad.h>
#include "Hazard/Rendering/Textures/Texture.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLUtils {
	public:
		static GLuint DataTypeToOpenGLType(TextureDataType type);
		static const char* GluintToString(GLuint value);
		static uint32_t DepthFuncToGL(DepthFunc value);
		static uint32_t CullFaceToGL(CullFace face);
	};
}