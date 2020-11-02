#pragma once
#include <hzrpch.h>
#include "OpenGLCubemapTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Hazard {

	OpenGLCubemapTexture::OpenGLCubemapTexture(const char* name, std::vector<std::string> sources) : CubemapTexture(name)
	{
		glGenTextures(1, &textureID);
		Revalidate(sources);
	}
	OpenGLCubemapTexture::~OpenGLCubemapTexture()
	{
		glDeleteTextures(1, &textureID);
	}
	void OpenGLCubemapTexture::Bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	}
	void OpenGLCubemapTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	void OpenGLCubemapTexture::Revalidate(std::vector<std::string> sources)
	{
		Bind();
		for (uint8_t i = 0; i < sources.size(); i++) {
			int w, h, channels;
			stbi_uc* data = stbi_load(sources[i].c_str(), &w, &h, &channels, 0);
			if (!data) {
				HZR_CORE_WARN("Cubemap file not found " + sources[i]);
				continue;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
}