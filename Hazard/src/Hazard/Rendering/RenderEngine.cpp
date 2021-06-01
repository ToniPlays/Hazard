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
	}
	RenderEngine::~RenderEngine()
	{

	}
	void RenderEngine::PreInit()
	{

	}
	void RenderEngine::InitRenderer(RenderEngineCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		RenderCommand::Init();

		RenderUtilsCreateInfo utilInfo = {};
		utilInfo.shaderCompilePath = info->shaderCompilePath;
		utilInfo.shaderSourcePath = info->shaderSourcePath;
		RenderUtils::Init(&utilInfo);

		m_Renderer2D = new Renderer2D(info);
		m_Renderer2D->Init(info->maxQuadCount);

		UniformBufferCreateInfo bufferInfo;
		bufferInfo.size = sizeof(CameraData);
		bufferInfo.binding = 0;

		m_CameraUnformBuffer = RenderUtils::Create<UniformBuffer>(bufferInfo);
	}
	void RenderEngine::Close()
	{
		HZR_PROFILE_FUNCTION();
	}

	void RenderEngine::BeginRendering(Camera camera, ECS::World& world)
	{
		RenderCommand::ResetStats();
		if(m_FrameBuffer != nullptr)
			m_FrameBuffer->Bind();

		m_BackgroundRenderer = world.GetWorldData().renderer;
		RenderContextCommand::ClearFrame(m_BackgroundRenderer->m_Color);

		m_Projection = camera.projection;
		m_View = camera.view;

		m_CameraData.viewProjection = camera.projection * glm::inverse(camera.view);
		m_CameraData.cameraPos = camera.position;
		m_CameraData.gamma = m_BackgroundRenderer->GetGamma();
		m_CameraUnformBuffer->SetData(&m_CameraData, sizeof(CameraData));

		m_Renderer2D->BeginScene(m_CameraData.viewProjection);
		m_Renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		m_Renderer2D->Flush();
		m_CameraData.viewProjection = m_Projection * glm::inverse(glm::mat4(glm::mat3(m_View)));
		m_CameraUnformBuffer->SetData(&m_CameraData, sizeof(CameraData));

		m_BackgroundRenderer->Render();
		if (m_FrameBuffer != nullptr)
			m_FrameBuffer->Unbind();
	}
}