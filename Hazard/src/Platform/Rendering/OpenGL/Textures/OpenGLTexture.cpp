#pragma once

#include <hzrpch.h>
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"

namespace Hazard {
	
	OpenGLTexture2D::OpenGLTexture2D(const char* file) : Texture2D(file)
	{
		int w, h, channels;
		stbi_uc* data = stbi_load(file, &w, &h, &channels, 0);
		if (!data) {
			HZR_CORE_WARN("Texture file not found");
			return;
		}
		width = w;
		height = h;

		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
		glTextureStorage2D(textureID, 1, GL_RGBA8, width, height);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(textureID, 0, 0, 0, width, height, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		
		//glGenerateMipmap(GL_TEXTURE_2D);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &textureID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, textureID);
	}

	void OpenGLTexture2D::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}

}