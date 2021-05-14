#pragma once

#include <hzrpch.h>
#include "OpenGLCubemapTexture.h"
#include "../OpenGLUtils.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/RenderCommand.h"
#include <glad/glad.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazard::Rendering::OpenGL {

	OpenGLCubemapTexture::OpenGLCubemapTexture(const CubemapCreateInfo& info) : CubemapTexture(info)
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		m_Info.datatype = info.datatype;
		m_Info.width = info.width;
		m_Info.height = info.height;
		
		if (info.sides.size() == 1) {
			AllocateFree();
			CreateCubemapFromTexture(info.sides.at(0).file);
		}
		else {
			for (CubemapSide side : info.sides) {
				SetTexture(side.side, side.file);
			}
		}

		SetFilters();
	}
	OpenGLCubemapTexture::~OpenGLCubemapTexture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void OpenGLCubemapTexture::SetTexture(int side, const std::string& file)
	{
		int w, h, channels;
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(file.c_str(), &w, &h, &channels, 0);

		uint32_t internalFormat = (channels == 4) * GL_RGBA8 + (channels == 3) * GL_RGB8;
		uint32_t dataFormat = (channels == 4) * GL_RGBA + (channels == 3) * GL_RGB;

		HZR_CORE_ASSERT(data, "Image could not be loaded");

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
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void OpenGLCubemapTexture::AllocateFree()
	{
		GLuint dataType = OpenGLUtils::DataTypeToOpenGLType(m_Info.datatype);

		for (uint8_t i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, dataType, 
				m_Info.width, m_Info.height, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		
	}
	void OpenGLCubemapTexture::CreateCubemapFromTexture(const std::string& file)
	{
		Texture2DCreateInfo textureCreateInfo;
		textureCreateInfo.filename = file;
		textureCreateInfo.datatype = TextureDataType::HDR;

		m_Info.environmentRawTexture = RenderUtils::Create<Texture2D>(textureCreateInfo);
		Ref<Shader> convertShader = RenderUtils::Create<Shader>("res/shaders/equirectangularToCube.glsl");
		Bind();

		float vertices[24] = {
			// positions
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f
		};
		uint32_t indices[36] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		BufferLayout layout = { { ShaderDataType::Float3, "pos" } };

		VertexBufferCreateInfo bufferInfo;
		bufferInfo.layout = &layout;
		bufferInfo.size = 24 * sizeof(float);
		bufferInfo.data = &vertices;

		IndexBufferCreateInfo indexBufferInfo;
		indexBufferInfo.size = 36;
		indexBufferInfo.data = indices;

		VertexArrayCreateInfo createInfo;
		createInfo.bufferInfo = &bufferInfo;
		createInfo.indexBufferInfo = &indexBufferInfo;

		VertexArray* cubeArray = RenderUtils::CreateRaw<VertexArray>(createInfo);

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		unsigned int captureFBO;
		unsigned int captureRBO;

		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Info.width, m_Info.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		glViewport(0, 0, m_Info.width, m_Info.height);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

		cubeArray->Bind();
		m_Info.environmentRawTexture->Bind();

		convertShader->Bind();
		convertShader->SetUniformInt("equirectangularMap", 0);
		convertShader->SetUniformMat4("projection", captureProjection);

		for (uint8_t i = 0; i < 6; ++i)
		{
			convertShader->SetUniformMat4("view", captureViews[i]);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureID, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RenderCommand::DrawIndexed(cubeArray, cubeArray->GetIndexBuffer()->GetCount());
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		HZR_CORE_WARN("HDR Cubemap finished");
	}
}