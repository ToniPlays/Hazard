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

		//m_EnvironmentMap->GenerateIrradiance();
		//m_EnvironmentMap->GeneratePreFilter();
	}
	void RenderEngine::Close()
	{

	}

	void RenderEngine::BeginRendering(Camera camera, BackgroundRenderer& renderer)
	{
		RenderCommand::ResetStats();
		m_RenderTarget->Bind();

		m_BackgroundRenderer = &renderer;
		m_ViewProjection = camera.projection * glm::inverse(camera.view);
		m_CameraPosition = camera.position;
		
		renderer.Render(camera.view, camera.projection);

		m_Renderer2D->BeginScene(m_ViewProjection);
		m_Renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		m_Renderer2D->Flush();
		m_RenderTarget->Unbind();
	}
}