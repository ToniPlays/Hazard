#pragma once

#include "Hazard/Math/Color.h"
#include <glad/glad.h>
#include "Hazard/Rendering/Textures/Texture.h"
#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"
#include "Hazard/Rendering/Pipeline/FrameBuffer.h"

#include <stb_image.h>

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLUtils {
	public:
		static GLuint DataTypeToOpenGLType(TextureDataType type);
		static const char* GluintToString(GLuint value);
		static uint32_t DepthFuncToGL(DepthFunc value);
		static uint32_t CullFaceToGL(CullFace face);
		static bool IsDepthFormat(FrameBufferTextureFormat format);
		static void CreateTextures(bool multisample, uint32_t* outID, uint32_t count);
		static void BindTexture(bool multisample, uint32_t id);
		static void AttachColorTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t width, uint32_t height, uint32_t iter);
		static void AttachDepthTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t attachmetType, uint32_t width, uint32_t height);

		static Color ReadPixel(size_t x, size_t y, stbi_uc* data, size_t width);
	private:
		static uint32_t TextureTarget(bool multisample);
	};
}