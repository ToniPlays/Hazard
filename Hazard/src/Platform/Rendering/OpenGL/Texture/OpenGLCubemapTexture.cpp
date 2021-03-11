#pragma once

#include <hzrpch.h>
#include "OpenGLCubemapTexture.h"
#include <glad/glad.h>

#include <stb_image.h>

namespace Hazard::Rendering::OpenGL {

	OpenGLCubemapTexture::OpenGLCubemapTexture()
	{
		std::vector<std::string> faces;
		faces.push_back("res/textures/sea-right.jpg");
		faces.push_back("res/textures/sea-left.jpg");
		faces.push_back("res/textures/sea-top.jpg");
		faces.push_back("res/textures/sea-bottom.jpg");
		faces.push_back("res/textures/sea-front.jpg");
		faces.push_back("res/textures/sea-back.jpg");

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int w, h, channels;
		for (uint8_t i = 0; i < faces.size(); i++) {
			unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &channels, 0);

			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0 , GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			stbi_image_free(data);
			HZR_CORE_INFO("Loading {0} to cubemap", faces[i].c_str());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	OpenGLCubemapTexture::~OpenGLCubemapTexture()
	{
		glDeleteTextures(1, &textureID);
	}
	unsigned int OpenGLCubemapTexture::GetWidth() const
	{
		return 0;
	}
	unsigned int OpenGLCubemapTexture::GetHeight() const
	{
		return 0;
	}
	void OpenGLCubemapTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, textureID);
	}
	void OpenGLCubemapTexture::Unbind(uint32_t slot) const
	{
		glBindTextureUnit(slot, 0);
	}
}