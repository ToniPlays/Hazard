#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"

#include "Hazard/Rendering/RenderUtils.h"
#include "Mesh/VertexData.h"
#include "RenderCommand.h"

#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEnginwe")
	{
		SetActive(true);
		std::cout << "RenderEngine()" << std::endl;
	}
	RenderEngine::~RenderEngine()
	{

	}
	void RenderEngine::PreInit()
	{
		
	}
	void RenderEngine::Init()
	{
		m_Renderer2D = new Renderer2D(&RenderContextCommand::GetContext());
		m_Renderer2D->Init(35000);

		RenderCommand::Init();
	}
	void RenderEngine::Close()
	{

	}

	void RenderEngine::BeginRendering(Camera camera)
	{
		RenderCommand::ResetStats();
		m_RenderTarget->Bind();
		RenderContextCommand::ClearFrame(camera.renderer->ClearColor());

		m_ViewProjection = camera.projection * glm::inverse(camera.view);
		m_CameraPosition = camera.position;
		
		camera.renderer->Render(camera.projection * glm::inverse(glm::mat4(glm::mat3(camera.view))));

		m_Renderer2D->BeginScene(m_ViewProjection);
		m_Renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		m_Renderer2D->Flush();
		m_RenderTarget->Unbind();
	}
}