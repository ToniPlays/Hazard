#pragma once

#include <hzrpch.h>
#include "OpenGLUtils.h"

namespace Hazard::Rendering::OpenGL 
{
	GLuint OpenGLUtils::DataTypeToOpenGLType(TextureDataType type)
	{
		switch (type)
		{
		case RGB:	return GL_RGB;
		case RGBA:	return GL_RGBA;
		case HDR:	return GL_RGB16F;
		case HDRA:	return GL_RGBA16F;
		}
	}
	const char* OpenGLUtils::GluintToString(GLuint value)
	{
		switch (value)
		{
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
		case GL_DEBUG_SEVERITY_LOW: return "Low";
		case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
		case GL_DEBUG_SEVERITY_HIGH: return "High";
		}
		return "";
	}
	uint32_t OpenGLUtils::DepthFuncToGL(DepthFunc value)
	{
		return GL_NEVER + value;
	}
	uint32_t OpenGLUtils::CullFaceToGL(CullFace face)
	{
		switch (face)
		{
		case None:				return GL_BACK;
		case FrontFace:			return GL_FRONT;
		case BackFace:			return GL_BACK;
		case FontAndBackFace:	return GL_FRONT_AND_BACK;
		}
	}
	bool OpenGLUtils::IsDepthFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::DEPTH32F:			return true;
		case FrameBufferTextureFormat::DEPTH32_STENCIL8:	return true;
		}
		return false;
	}
	void OpenGLUtils::CreateTextures(bool multisample, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(multisample), count, outID);
	}
	void OpenGLUtils::BindTexture(bool multisample, uint32_t id)
	{
		glBindTexture(TextureTarget(multisample), id);
	}
	void OpenGLUtils::AttachColorTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t width, uint32_t height, uint32_t iter)
	{
		bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + iter, TextureTarget(multisampled), target, 0);
	}
	void OpenGLUtils::AttachDepthTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t attachmetType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmetType, TextureTarget(multisampled), target, 0);
	}
	Color OpenGLUtils::ReadPixel(size_t x, size_t y, stbi_uc* data, size_t width)
	{
		const stbi_uc* p = data + ((size_t)4 * (y * width + x));

		float r = p[0];
		float g = p[1];
		float b = p[2];
		float a = p[3];

		return Color(r, g, b, a);
	}
	uint32_t OpenGLUtils::TextureTarget(bool multisample)
	{
		return multisample ? GL_TEXTURE_2D_MULTISAMPLE: GL_TEXTURE_2D;
	}
}