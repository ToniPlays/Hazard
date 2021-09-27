#pragma once
#include <hzrpch.h>
#include "RenderContext.h"

#include "Hazard/Core/ApplicationCreateInfo.h"

#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) : Module::Module("RenderContext")
	{
		m_CurrentAPI = info->Renderer == RenderAPI::Auto ? RenderAPI::OpenGL : info->Renderer;
		m_ClearColor = info->Color;

		m_Window = Window::Create(info, appInfo);
		m_Window->SetEventCallback(BIND_EVENT(RenderContext::Process));

		if (appInfo->IconCount > 0)
			m_Window->SetWindowIcon(appInfo->IconCount, appInfo->Icons);

		RenderContextCommand::Init(this);
		m_Window->GetContext()->SetClearColor({ info->Color.r, info->Color.g, info->Color.b, 1.0f });
		SetActive(true);
	}
	void RenderContext::Update()
	{
		HZR_PROFILE_FUNCTION();
		m_Window->OnUpdate();
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
		case RenderAPI::Auto:	return "Auto";
		case RenderAPI::OpenGL:	return "OpenGL";
		case RenderAPI::Vulkan: return "Vulkan";
		case RenderAPI::DX11:	return "DX11";
		case RenderAPI::DX12:	return "DX12";
		case RenderAPI::Metal:	return "Metal";
		}
		return "Auto";
	}
}