#pragma once

#include "Hazard/Core/Core.h"
#include <hzrpch.h>
#include "Hazard/Rendering/Pipeline/Shader.h"
#include <glad/glad.h>
#include <shaderc/shaderc.h>

namespace Hazard::Rendering::OpenGLUtils 
{
	static GLenum ShaderTypeToGLType(ShaderType type) {
		switch (type)
		{
		case Vertex:	return GL_VERTEX_SHADER;
		case Fragment:	return GL_FRAGMENT_SHADER;
		case Compute:	return GL_COMPUTE_SHADER;
		case Geometry:	return GL_GEOMETRY_SHADER;
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown ShaderType {0}", Utils::ShaderTypeToString(type));
		return 0;
	}
	static ShaderType ShaderTypeFromGLType(GLenum type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:		return Vertex;
		case GL_FRAGMENT_SHADER:	return Fragment;
		case GL_COMPUTE_SHADER:		return Compute;
		case GL_GEOMETRY_SHADER:	return Geometry;
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown conversion from glType to ShaderType");
		return ShaderType::Unknown;
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

	static std::string GLShaderStageCachedVulkanExtension(ShaderType type) {
		switch (type)
		{
		case Vertex:	return ".cached_vulkan.vert";
		case Fragment:	return ".cached_vulkan.frag";
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown Vulkan cache filename for {0}", Utils::ShaderTypeToString(type));
		return "";
	}
	static std::string GLShaderStageCachedOpenGLExtension(GLenum type) {
		switch (type)
		{
		case GL_VERTEX_SHADER:		return ".cached_opengl.vert";
		case GL_FRAGMENT_SHADER:	return ".cached_opengl.frag";
		}
		HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown OpenGL cache filename for {0}", GLTypeToString(type));
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
}