#pragma once
#include <hzrpch.h>
#include "RenderContext.h"

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"

#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext() : Module::Module("RenderContext")
	{
		
	}
	RenderContext::~RenderContext() {}

	void RenderContext::InitContext(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo)
	{
		m_CurrentAPI = info->renderer == RenderAPI::Auto ? RenderAPI::OpenGL : info->renderer;
		RenderUtils::SetRenderAPI(m_CurrentAPI);

		m_Window = Window::Create(info, appInfo);
		m_Window->SetEventCallback(BIND_EVENT(RenderContext::Process));

		if (appInfo->iconCount > 0) {
			m_Window->SetWindowIcon(appInfo->iconCount, appInfo->icons);
		}

		RenderContextCommand::Init();
		SetActive(true);
	}

	void RenderContext::Update()
	{
		HZR_PROFILE_FUNCTION();
		m_Window->OnUpdate(m_ClearColor);
	}

	void RenderContext::Close()
	{
		HZR_PROFILE_FUNCTION();
		delete m_Window;
	}

	void RenderContext::Process(Event& e)
	{
		Core::HazardLoop::GetCurrent().Process(e);
	}
	const char* RenderContext::APIToString(RenderAPI api)
	{
		switch (api)
		{
		case Hazard::RenderAPI::OpenGL:	return "OpenGL";
		case Hazard::RenderAPI::Vulkan: return "Vulkan";
		case Hazard::RenderAPI::DX11:	return "DX11";
		case Hazard::RenderAPI::DX12:	return "DX12";
		}
		return "Auto";
	}
}