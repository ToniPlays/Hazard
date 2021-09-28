#pragma once

#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderCommand.h"
#include "Loaders.h"
#include "Hazard/Assets/AssetManager.h"
#include "2D/Renderer2D.h"

namespace Hazard::Rendering 
{
	RenderEngine::RenderEngine(RenderEngineCreateInfo* info, RenderAPI api) : Module("RenderEngine")
	{
		SetActive(true);
		RenderCommand::m_Engine = this;
		RenderCommand::s_Api = api;
		AssetManager::RegisterLoader<TextureLoader>(AssetType::Texture);
		AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);

		m_ShaderCompilePath = info->ShaderCompilePath;
		m_ShaderSourcePath = info->ShaderSourcePath;

		m_Renderer2D = new Renderer2D(info);
	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
	}
	void RenderEngine::Update()
	{
		m_Renderer2D->Update();
	}
}
