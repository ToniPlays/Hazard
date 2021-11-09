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

		RenderCommand::s_Engine = this;
		RenderCommand::s_Api = api;

		m_ShaderCompilePath = info->ShaderCompilePath;
		m_ShaderSourcePath = info->ShaderSourcePath;

		//AssetManager::RegisterLoader<TextureLoader>(AssetType::Image);
		//AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);

		uint32_t data = 0xFFFFFFFF;

		TextureFilter filters;
		filters.MinFilter = FilterMode::Nearest;
		filters.MagFilter = FilterMode::Nearest;

		Texture2DCreateInfo whiteTexture = {};
		whiteTexture.FilePath = "res/textures/checker.png";
		whiteTexture.Filter = &filters;
		whiteTexture.Width = 1;
		whiteTexture.Height = 1;

		//whiteTexture.Data = &data;

		//m_WhiteTexture = Texture2D::Create(&whiteTexture);

		m_Renderer2D = new Renderer2D(info);

		WindowResizeEvent e = { 1920, 1080 };
		OnResize(e);

	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
	}
	void RenderEngine::Render()
	{
		RenderPassData data = {};

		float sin = (1 + Math::Sin(Time::s_Time) * 0.5f);
		data.Transform = glm::inverse(glm::translate(glm::mat4(1.0f), { sin, 1.0f - sin, 4.0f }));

		data.ViewProjection = m_ViewProjection * data.Transform;

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
		if (e.GetWidth() == 0 || e.GetHeight() == 0) return false;

		constexpr float size = 2.0f;
		float aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		float scalar = aspectRatio * size;

		//m_ViewProjection = glm::ortho(-scalar, scalar, -size, size, -1000.0f, 1000.0f);
		m_ViewProjection = glm::perspective(glm::radians(80.0f), aspectRatio, 0.003f, 1000.0f);
		return true;
	}
}
