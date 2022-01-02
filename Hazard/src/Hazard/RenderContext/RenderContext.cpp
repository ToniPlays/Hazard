
#include <hzrpch.h>
#include "RenderContext.h"

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) : Module::Module("RenderContext")
	{
		HZR_PROFILE_FUNCTION();
		m_CurrentAPI = info->Renderer == RenderAPI::Auto ? RenderAPI::Vulkan : info->Renderer;
		m_ClearColor = info->Color;
		m_ImagesInFlight = info->ImagesInFlight;
		RenderContextCommand::Init(this);

		m_Window = Window::Create(info, appInfo);
		m_Window->SetEventCallback(BIND_EVENT(RenderContext::Process));

		if (appInfo->IconCount > 0)
			m_Window->SetWindowIcon(appInfo->IconCount, appInfo->Icons);


		m_Window->GetContext()->SetClearColor({ info->Color.r, info->Color.g, info->Color.b, 1.0f });
		SetActive(true);
	}
	void RenderContext::PreRender()
	{
		m_Window->OnBeginFrame();
	}
	void RenderContext::PostRender() {
		m_Window->OnEndFrame();
	}

	void RenderContext::Close()
	{
		HZR_PROFILE_FUNCTION();
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
		default:
			return "Auto";
		}
		return "Auto";
	}
	RenderAPI RenderContext::StringToApi(const std::string& api)
	{
		if (api == "Auto")		return RenderAPI::Auto;
		if (api == "OpenGL")	return RenderAPI::OpenGL;
		if (api == "Vulkan")	return RenderAPI::Vulkan;
		if (api == "DX11")		return RenderAPI::DX11;
		if (api == "DX12")		return RenderAPI::DX12;
		if (api == "Metal")		return RenderAPI::Metal;
		return RenderAPI::Auto;
	}
}
