#pragma once
#include <hzrpch.h>
#include "RenderContext.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"

#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext() : Module::Module("RenderContext")
	{
		
	}
	RenderContext::~RenderContext() {}

	void RenderContext::InitContext(RenderAPI api)
	{
		m_CurrentAPI = api == RenderAPI::Auto ? RenderAPI::OpenGL : api;
		RenderUtils::SetRenderAPI(m_CurrentAPI);

		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT(RenderContext::Process));

		RenderContextCommand::Init();
		SetActive(true);
	}

	void RenderContext::Update()
	{
		m_Window->OnUpdate(m_ClearColor);
	}

	void RenderContext::Close()
	{
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
		case Hazard::Rendering::RenderAPI::OpenGL:	return "OpenGL";
		case Hazard::Rendering::RenderAPI::Vulkan:  return "Vulkan";
		case Hazard::Rendering::RenderAPI::DX11:	return "DX11";
		case Hazard::Rendering::RenderAPI::DX12:	return "DX12";
		}
		return "Auto";
	}
}