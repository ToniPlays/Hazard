#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "2D/QuadData.h"
#include "RenderCommand.h"

namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		SetActive(true);
	}
	RenderEngine::~RenderEngine()
	{

	}
	void RenderEngine::PreInit()
	{
		
	}
	void RenderEngine::Init()
	{
		renderer2D = new Renderer2D(&RenderContextCommand::GetContext());
		renderer2D->Init(35000);

		skybox = RenderUtils::Create<CubemapTexture>();

		skyboxVao = RenderUtils::Create<VertexArray>();
		
		float skyboxVertices[] = {
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
		VertexBuffer* buffer = RenderUtils::Create<VertexBuffer>((uint32_t)(24* sizeof(float)));

		buffer->SetData(skyboxVertices, 24 * sizeof(float));
		
		buffer->SetLayout({ {ShaderDataType::Float3, "skybox_pos"} });
		skyboxVao->AddBuffer(buffer);

		uint32_t indices[] = {
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		IndexBuffer* iBuffer = RenderUtils::Create<IndexBuffer>();
		iBuffer->SetData(indices, 36);
		skyboxVao->SetIndexBuffer(iBuffer);

		skyboxShader = RenderUtils::Create<Shader>("res/shaders/skybox.glsl");
		skyboxShader->Bind();
		skyboxShader->SetUniformInt("SkyboxCubemap", 0);
		skyboxShader->Unbind();

		skybox->Unbind();

		RenderCommand::Init();
	}
	void RenderEngine::Close()
	{
		renderer2D->Close();
	}
	void RenderEngine::BeginRendering(Camera camera)
	{
		skyboxShader->Bind();
		skyboxShader->SetUniformMat4("viewProjection", camera.projection * glm::inverse(glm::mat4(glm::mat3(camera.view))));
		skyboxShader->Unbind();

		RenderCommand::ResetStats();
		renderTarget->Bind();
		RenderContextCommand::ClearFrame(camera.clearColor);

		renderer2D->BeginScene(camera.projection * glm::inverse(camera.view));
		renderer2D->BeginBatch();
	}
	void RenderEngine::EndRendering()
	{
		renderer2D->Flush();

		//Skybox
		glDepthFunc(GL_LEQUAL);

		skyboxVao->Bind();
		skyboxShader->Bind();
		skybox->Bind();

		RenderCommand::DrawIndexed(skyboxVao, 36);
		glDepthFunc(GL_LESS);
		renderTarget->Unbind();
	}
}