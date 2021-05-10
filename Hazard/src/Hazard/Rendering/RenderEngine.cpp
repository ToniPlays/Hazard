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

		TextureSpecs specs;
		specs.width = 2048;
		specs.height = 2048;

		m_Skybox = new Skybox();
		m_EnvironmentMap = RenderUtils::Create<EnvinronmentMap>(File::GetFileAbsolutePath("res/textures/modern_buildings_8k.hdr").c_str(), specs);
		m_Skybox->SetCubemapTexture(m_EnvironmentMap);

		//m_EnvironmentMap->GenerateIrradiance();
		//m_EnvironmentMap->GeneratePreFilter();
	}
	void RenderEngine::Close()
	{

	}

	void RenderEngine::BeginRendering(Camera camera)
	{
		RenderCommand::ResetStats();
		m_RenderTarget->Bind();
		RenderContextCommand::ClearFrame(camera.clearColor);

		m_ViewProjection = camera.projection * glm::inverse(camera.view);
		m_CameraPosition = camera.position;

		m_Skybox->Render(camera.projection * glm::inverse(glm::mat4(glm::mat3(camera.view))));

		m_Renderer2D->BeginScene(m_ViewProjection);
		m_Renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		m_Renderer2D->Flush();
		m_RenderTarget->Unbind();
	}
}