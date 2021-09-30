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
		HZR_PROFILE_FUNCTION();
		SetActive(true);

		RenderCommand::m_Engine = this;
		RenderCommand::s_Api = api;
		AssetManager::RegisterLoader<TextureLoader>(AssetType::Texture);
		AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);

		m_ShaderCompilePath = info->ShaderCompilePath;
		m_ShaderSourcePath = info->ShaderSourcePath;

		m_Renderer2D = new Renderer2D(info);

		WindowResizeEvent e = { 1280, 720 };
		OnResize(e);

	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
	}
	void RenderEngine::Render()
	{
		RenderPassData data;
		data.ViewProjection = m_ViewProjection;
		float sin = (1 + Math::Sin(Time::s_Time) * 0.5f);
		data.Transform = glm::inverse(glm::translate(glm::mat4(1.0f), { sin, 1.0f - sin, 0.0f }));

		m_Renderer2D->Render(data);
	}
	bool RenderEngine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(RenderEngine::OnResize));
	}
	void RenderEngine::Close()
	{
		HZR_PROFILE_FUNCTION();
	}
	bool RenderEngine::OnResize(WindowResizeEvent& e)
	{
		constexpr float size = 2.0f;
		float aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		aspectRatio *= size;

		m_ViewProjection = glm::ortho(-aspectRatio, aspectRatio, -size, size, -1.0f, 1.0f);

		return true;
	}
}
