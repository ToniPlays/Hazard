#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_OPENGL

#include "Backend/Core/Texture/Image.h"
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/Shader.h"

#include "Severity.h"

#include <glad/glad.h>
#include <shaderc/shaderc.h>

namespace HazardRenderer::OpenGLUtils
{
	static GLenum ShaderStageToGLType(ShaderStage type) {
		switch (type)
		{
		case ShaderStage::Vertex:	return GL_VERTEX_SHADER;
		case ShaderStage::Fragment:	return GL_FRAGMENT_SHADER;
		case ShaderStage::Compute:	return GL_COMPUTE_SHADER;
		case ShaderStage::Geometry:	return GL_GEOMETRY_SHADER;
		default:
			HZR_ASSERT(false, "Unknow ShaderType");
		}
		HZR_ASSERT(false, "Unknow ShaderType");
		return 0;
	}
	static ShaderStage ShaderStageFromGLType(GLenum type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:		return ShaderStage::Vertex;
		case GL_FRAGMENT_SHADER:	return ShaderStage::Fragment;
		case GL_COMPUTE_SHADER:		return ShaderStage::Compute;
		case GL_GEOMETRY_SHADER:	return ShaderStage::Geometry;
		}
		HZR_ASSERT(false, "[OpenGLShader]: Unkown conversion from glType to ShaderType");
		return ShaderStage::None;
	}
	static std::string GLTypeToString(GLenum type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:		return "Vertex";
		case GL_FRAGMENT_SHADER:	return "Fragment";
		case GL_COMPUTE_SHADER:		return "Compute";
		case GL_GEOMETRY_SHADER:	return "Geometry";
		}
		HZR_ASSERT(false, "[OpenGLShader]: Unknown conversion from GLenum to string");
		return "";
	}
	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
		switch (stage)
		{
		case GL_VERTEX_SHADER:		return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
		}
		HZR_ASSERT(false, "[OpenGLShader]: Unkown OpenGL type to ShaderC type");
		return (shaderc_shader_kind)0;
	}
	static GLenum DrawTypeToGLType(const DrawType& type) {
		switch (type)
		{
		case DrawType::Fill: return GL_TRIANGLES;
		case DrawType::Line: return GL_LINES;
		case DrawType::Point: return GL_POINTS;
		default:
			HZR_ASSERT(false, "Unknow DrawType");
		}
		return GL_TRIANGLES;
	}
	static GLenum DrawTypeToGLPolygon(const DrawType& type) {
		switch (type)
		{
		case DrawType::Fill: return GL_FILL;
		case DrawType::Line: return GL_LINE;
		case DrawType::Point: return GL_POINT;
		default: HZR_ASSERT(false, "Unkown DrawType");

		}
		return GL_TRIANGLES;
	}
	static Severity GLuintToSeverity(GLuint severity) {
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_NOTIFICATION:	return Severity::Info;
		case GL_DEBUG_SEVERITY_LOW:				return Severity::Debug;
		case GL_DEBUG_SEVERITY_MEDIUM:			return Severity::Warning;
		case GL_DEBUG_SEVERITY_HIGH:			return Severity::Error;
		}
		return Severity::Info;
	}
	static GLuint GetFormatType(uint32_t format)
	{
		switch (format) {
		case GL_RGB16F:		return GL_FLOAT;
		case GL_RGBA16F:	return GL_FLOAT;
		default:			return GL_UNSIGNED_BYTE;
		}
	}
	static GLuint GetGLFormat(const ImageFormat& format)
	{
		switch (format)
		{
		case ImageFormat::RED32F:			return GL_R32F;
		case ImageFormat::RGB:				return GL_RGB8;
		case ImageFormat::RGB16F:			return GL_RGB16F;
		case ImageFormat::RGB32F:			return GL_RGB32F;
		case ImageFormat::RGBA:				return GL_RGBA8;
		case ImageFormat::RGBA16F:			return GL_RGBA16F;
		case ImageFormat::RGBA32F:			return GL_RGBA32F;
		case ImageFormat::RG16F:			return GL_RG16F;
		case ImageFormat::RG32F:			return GL_RG32F;
		case ImageFormat::SRGB:				return GL_RGBA32F;
		case ImageFormat::RED32I:			return GL_R32I;
		case ImageFormat::DEPTH32F:			return GL_DEPTH32F_STENCIL8;
		case ImageFormat::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
		}
		return GL_RGBA8;
	}

	static GLuint GetGLDepthFunc(const DepthOp& op)
	{
		switch (op)
		{
		case DepthOp::Never:			return GL_NEVER;
		case DepthOp::NotEqual:			return GL_NOTEQUAL;
		case DepthOp::Less:				return GL_LESS;
		case DepthOp::LessOrEqual:		return GL_LEQUAL;
		case DepthOp::Greater:			return GL_GREATER;
		case DepthOp::GreaterOrEqual:	return GL_GEQUAL;
		case DepthOp::Equal:			return GL_EQUAL;
		case DepthOp::Always:			return GL_ALWAYS;
		}
		return GL_LESS;
	}



	static bool IsDepthFormat(const ImageFormat& format) {
		if (format == ImageFormat::DEPTH24STENCIL8 || format == ImageFormat::DEPTH32F)
			return true;
		return false;
	}
	static uint32_t TextureTarget(bool multisample)
	{
		return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}
	static uint32_t CreateTextures(bool multisample, uint32_t count)
	{
		uint32_t id;
		glCreateTextures(TextureTarget(multisample), count, &id);
		std::cout << "Created texture " << id << std::endl;
		return id;
	}
	static void BindTexture(uint32_t id, bool multisampled) 
	{
		glBindTexture(TextureTarget(multisampled), id);
	}
	static void AttachColorTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t internalFormat, uint32_t width, uint32_t height, uint32_t iter)
	{
		//Change to DSA
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTextureStorage2D(target, 1, internalFormat, width, height);
		}

		glTextureParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + iter, TextureTarget(multisampled), target, 0);
	}
	static void AttachDepthTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t attachmetType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTextureStorage2D(target, 1, format, width, height);
		}
		glTextureParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTextureParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmetType, TextureTarget(multisampled), target, 0);
	}

}
#endif
