#pragma once

#include <hzrpch.h>
#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Image/Image.h"
#include "Hazard/RenderContext/Pipeline/Pipeline.h"
#include <glad/glad.h>
#include <shaderc/shaderc.h>

namespace Hazard::Rendering::OpenGLUtils 
{
	static GLenum ShaderTypeToGLType(ShaderType type) {
		switch (type)
		{
		case ShaderType::Vertex:	return GL_VERTEX_SHADER;
		case ShaderType::Fragment:	return GL_FRAGMENT_SHADER;
		case ShaderType::Compute:	return GL_COMPUTE_SHADER;
		case ShaderType::Geometry:	return GL_GEOMETRY_SHADER;
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown ShaderType {0}", Utils::ShaderTypeToString(type));
		return 0;
	}
	static ShaderType ShaderTypeFromGLType(GLenum type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:		return ShaderType::Vertex;
		case GL_FRAGMENT_SHADER:	return ShaderType::Fragment;
		case GL_COMPUTE_SHADER:		return ShaderType::Compute;
		case GL_GEOMETRY_SHADER:	return ShaderType::Geometry;
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown conversion from glType to ShaderType");
		return ShaderType::None;
	}
	static std::string GLTypeToString(GLenum type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:		return "Vertex";
		case GL_FRAGMENT_SHADER:	return "Fragment";
		case GL_COMPUTE_SHADER:		return "Compute";
		case GL_GEOMETRY_SHADER:	return "Geometry";
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unknown conversion from GLenum to string");
		return "";
	}
	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
		switch (stage)
		{
		case GL_VERTEX_SHADER:		return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown OpenGL type to ShaderC type");
		return (shaderc_shader_kind)0;
	}
	static GLenum DrawTypeToGLType(const DrawType& type) {
		switch (type)
		{
		case DrawType::Fill: return GL_TRIANGLES;
		case DrawType::Line: return GL_LINES;
		case DrawType::Point: return GL_POINTS;
		}
		return GL_TRIANGLES;
	}
	static GLenum DrawTypeToGLPolygon(const DrawType& type) {
		switch (type)
		{
		case DrawType::Fill: return GL_FILL;
		case DrawType::Line: return GL_LINE;
		case DrawType::Point: return GL_POINT;
		}
		return GL_TRIANGLES;
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
	static void CreateTextures(bool multisample, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(TextureTarget(multisample), count, outID);
	}
	static void BindTexture(uint32_t id, bool multisampled) {
		glBindTexture(TextureTarget(multisampled), id);
	}
	static void AttachColorTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t width, uint32_t height, uint32_t iter)
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
	static void AttachDepthTexture(uint32_t target, uint32_t samples, uint32_t format, uint32_t attachmetType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
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
}