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

	}

	void RenderEngine::BeginRendering(Camera camera, BackgroundRenderer& renderer)
	{

		RenderCommand::ResetStats();
		m_RenderTarget->Bind();
		RenderContextCommand::ClearFrame(renderer.m_Color);

		m_BackgroundRenderer = &renderer;
		cameraData.viewProjection = camera.projection * glm::inverse(camera.view);
		cameraData.cameraPos = camera.position;

		m_CameraUnformBuffer->SetData(&cameraData, sizeof(CameraData));


		m_Projection = camera.projection;
		m_View = camera.view;

		m_Renderer2D->BeginScene(cameraData.viewProjection);
		m_Renderer2D->BeginBatch();
	}
		
	void RenderEngine::EndRendering()
	{
		m_Renderer2D->Flush();
		m_BackgroundRenderer->Render(m_View, m_Projection);
		m_RenderTarget->Unbind();
	}
}