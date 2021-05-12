#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"

#include "Hazard/Rendering/RenderUtils.h"
#include "Mesh/VertexData.h"
#include "RenderCommand.h"
#include "Hazard/File/File.h"

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
		RenderCommand::Init();
		m_Renderer2D = new Renderer2D(&RenderContextCommand::GetContext());
		m_Renderer2D->Init(35000);
	}
	void RenderEngine::Close()
	{

	}

	void RenderEngine::BeginRendering(Camera camera, BackgroundRenderer& renderer)
	{
		RenderCommand::ResetStats();
		m_RenderTarget->Bind();
		RenderContextCommand::ClearFrame(renderer.m_Color);

		m_BackgroundRenderer = &renderer;
		m_ViewProjection = camera.projection * glm::inverse(camera.view);

		m_CameraPosition = camera.position;
		m_Projection = camera.projection;
		m_View = camera.view;

		m_Renderer2D->BeginScene(m_ViewProjection);
		m_Renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		m_Renderer2D->Flush();
		m_BackgroundRenderer->Render(m_View, m_Projection);
		m_RenderTarget->Unbind();
	}
}