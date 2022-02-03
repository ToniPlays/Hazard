
#include <hzrpch.h>
#include "RenderContext.h"

#include "Hazard/Core/ApplicationCreateInfo.h"
#include "RenderContextCommand.h"
#include "RenderLibrary.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderContexCreateInfo* info, ApplicationCreateInfo* appInfo) : Module::Module("RenderContext")
	{
		m_ClearColor = info->Color;
		m_ImagesInFlight = info->ImagesInFlight;
		RenderContextCommand::Init(this);

		m_Window = Window::Create(info, appInfo);
		m_Window->SetEventCallback(BIND_EVENT(RenderContext::Process));

        m_CurrentAPI = m_Window->GetWindowInfo().SelectedAPI;
        HZR_CORE_ASSERT(m_CurrentAPI != RenderAPI::Auto, "RenderAPI cannot be auto after window creation");
        
		if (appInfo->IconCount > 0)
			m_Window->SetWindowIcon(appInfo->IconCount, appInfo->Icons);


		m_Window->GetContext()->SetClearColor({ info->Color.r, info->Color.g, info->Color.b, 1.0f });
		SetActive(true);

		SetupQueues();
	}
	RenderContext::~RenderContext()
	{
		
	}
	void RenderContext::Render()
	{
		m_Window->GetContext()->BeginFrame();
	}
	void RenderContext::PostRender()
	{
		HZR_PROFILE_FUNCTION();

		m_ResourceFreeCommadQueue->Excecute();
		m_ResourceCreateCommadQueue->Excecute();
		m_RenderCommadQueue->Excecute();

		m_Window->Present();

		m_ResourceFreeCommadQueue->Clear();
		m_ResourceCreateCommadQueue->Clear();
		m_RenderCommadQueue->Clear();
	}

	void RenderContext::Close()
	{
		RenderLibrary::Clear();
		m_Window->Close();

		m_ResourceCreateCommadQueue->Excecute();
		m_ResourceFreeCommadQueue->Excecute();

		delete m_RenderCommadQueue;
		delete m_ResourceCreateCommadQueue;
		delete m_ResourceFreeCommadQueue;

		delete m_Window;
	}

	void RenderContext::Process(Event& e)
	{
		Core::HazardLoop::GetCurrent().Process(e);
	}
	void RenderContext::SetupQueues()
	{
		m_RenderCommadQueue = new CommandQueue();
		m_ResourceCreateCommadQueue = new CommandQueue();
		m_ResourceFreeCommadQueue = new CommandQueue();
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
