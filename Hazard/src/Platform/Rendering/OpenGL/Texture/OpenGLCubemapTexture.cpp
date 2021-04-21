#pragma once

#include <hzrpch.h>
#include "OpenGLCubemapTexture.h"
#include <glad/glad.h>

#include <stb_image.h>

namespace Hazard::Rendering::OpenGL {

	OpenGLCubemapTexture::OpenGLCubemapTexture() : CubemapTexture("")
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
		SetFilters();
	}
	OpenGLCubemapTexture::OpenGLCubemapTexture(const char* file) : CubemapTexture(file)
	{
		HZR_CORE_ERROR("Not implemented");
	}
	OpenGLCubemapTexture::OpenGLCubemapTexture(std::vector<std::string>& faces) : CubemapTexture(faces[0].c_str())
	{
		
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		for (uint8_t i = 0; i < faces.size(); i++) {
			SetTexture(i, faces[i]);
		}

		SetFilters();
	}
	OpenGLCubemapTexture::OpenGLCubemapTexture(const std::string& name, const std::string& extension) : CubemapTexture(name.c_str())
	{
		std::vector<std::string> sides;
		sides.push_back(name + "right" + extension);
		sides.push_back(name + "left" + extension);
		sides.push_back(name + "top" + extension);
		sides.push_back(name + "bottom" + extension);
		sides.push_back(name + "front" + extension);
		sides.push_back(name + "back" + extension);

		for (uint8_t i = 0; i < sides.size(); i++) {
			SetTexture(i, sides[i]);
		}
		SetFilters();
	}
	OpenGLCubemapTexture::~OpenGLCubemapTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	unsigned int OpenGLCubemapTexture::GetWidth() const
	{
		return 0;
	}
	unsigned int OpenGLCubemapTexture::GetHeight() const
	{
		return 0;
	}
	void OpenGLCubemapTexture::SetTexture(int side, const std::string& file)
	{
		int w, h, channels;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(file.c_str(), &w, &h, &channels, 0);
		uint32_t internalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;
		uint32_t dataFormat = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, internalFormat, w, h, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
	}
	void OpenGLCubemapTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_TextureID);
	}
	void OpenGLCubemapTexture::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
	void OpenGLCubemapTexture::SetFilters()
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}