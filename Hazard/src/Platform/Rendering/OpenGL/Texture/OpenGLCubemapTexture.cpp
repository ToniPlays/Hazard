#pragma once

#include <hzrpch.h>
#include "OpenGLCubemapTexture.h"
#include "../OpenGLUtils.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "../Pipeline/OpenGLGraphicsPipeline.h"
#include <glad/glad.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazard::Rendering::OpenGL {

	OpenGLCubemapTexture::OpenGLCubemapTexture(const CubemapCreateInfo& info)
	{
		m_TextureInfo.file = info.sides.at(0).file;
		m_Info.datatype = info.datatype;
		m_Info.width = info.width;
		m_Info.height = info.height;

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

		if (info.sides.size() == 1) {
			AllocateFree();
			CreateCubemapFromTexture(info.sides.at(0).file);
		}
		else 
		{
			m_Info.cubeSides.resize(info.sides.size());
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
		m_Info.cubeSides[side] = Ref<Texture2D>();

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
		HZR_PROFILE_FUNCTION();
		Texture2DCreateInfo textureCreateInfo;
		textureCreateInfo.filename = file;
		textureCreateInfo.datatype = TextureDataType::HDR;
		m_Info.cubeSides.clear();
		m_Info.cubeSides.push_back(RenderUtils::Create<Texture2D>(textureCreateInfo));
		if (file == "") return;
#pragma region Nonsense
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
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

#pragma endregion

		PipelineRasterizer rasterizer = {};
		rasterizer.cullFace = CullFace::BackFace;
		rasterizer.depthFunc = DepthFunc::Always;

		PipelineViewport viewport;
		viewport.offset = { 0, 0 };
		viewport.size = { m_Info.width, m_Info.height };

		PipelineShaderStage stages[2];
		stages[0].shaderFileName = "equirectangularToCubemap_vert.glsl";
		stages[0].fileType = ShaderFileType::Source;
		stages[0].stage = ShaderType::VertexShader;

		stages[1].shaderFileName = "equirectangularToCubemap_frag.glsl";
		stages[1].fileType = ShaderFileType::Source;
		stages[1].stage = ShaderType::FragmentShader;

		GraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.shaderPipelineName = "EquirectangularMap";
		pipelineInfo.viewport = &viewport;
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;

		GraphicsPipeline* pipeline = RenderUtils::CreateRaw<GraphicsPipeline>(pipelineInfo);

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

		unsigned int captureFBO;
		unsigned int captureRBO;

		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Info.width, m_Info.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		//pipeline.Bind();
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glViewport(0, 0, m_Info.width, m_Info.height);

		cubeArray->Bind();
		m_Info.cubeSides.at(0)->Bind();
		pipeline->GetShader()->Bind();
		pipeline->GetShader()->SetUniformInt("u_EquirectangularMap", 0);
		pipeline->GetShader()->SetUniformMat4("u_Projection", captureProjection);

		for (uint8_t i = 0; i < 6; ++i)
		{
			pipeline->GetShader()->SetUniformMat4("u_View", captureViews[i]);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_TextureID, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			RenderCommand::DrawIndexed(cubeArray, cubeArray->GetIndexBuffer()->GetInfo().count);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		HZR_CORE_WARN("HDR Cubemap finished");
		delete pipeline;
	}
}