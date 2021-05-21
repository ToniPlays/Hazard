#pragma once

#include "Hazard/Math/Color.h"
#include <glad/glad.h>
#include "Hazard/Rendering/Textures/Texture.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"

#include <stb_image.h>

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLUtils {
	public:
		static GLuint DataTypeToOpenGLType(TextureDataType type);
		static const char* GluintToString(GLuint value);
		static uint32_t DepthFuncToGL(DepthFunc value);
		static uint32_t CullFaceToGL(CullFace face);

		static Color ReadPixel(size_t x, size_t y, stbi_uc* data, size_t width);
	};
}