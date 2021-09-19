#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"

#include "Hazard/Rendering/RenderUtils.h"
#include "Mesh/VertexData.h"
#include "RenderCommand.h"
#include "Hazard/File/File.h"
#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering {

	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		SetActive(true);
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

	void RenderEngine::BeginRendering(const Camera& camera, ECS::World& world)
	{
		RenderCommand::ResetStats();
	}
		
	void RenderEngine::EndRendering()
	{
		
	}
}