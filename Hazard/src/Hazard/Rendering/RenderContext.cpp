#pragma once
#include <hzrpch.h>
#include "RenderContext.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"

#include "Hazard/Core/HazardLoop.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderAPI api) : currentAPI(api), Module::Module("Window")
	{
		
	}
	RenderContext::~RenderContext()
	{
		Close();
	}

	void RenderContext::Init()
	{
		window = Window::Create();
		window->SetEventCallback(BIND_EVENT(RenderContext::Process));
	}

	void RenderContext::Update()
	{
		window->OnUpdate();
	}

	void RenderContext::Close()
	{
		
	}

	const char* RenderContext::ApiAsString(RenderAPI api)
	{
		switch (api)
		{
		case RenderAPI::OpenGL:	return "OpenGL";
		case RenderAPI::Vulkan: return "Vulkan";
		default:				return "Auto";
		}
	}
	void RenderContext::Process(Event& e)
	{
		Core::HazardLoop::GetCurrent().Process(e);
	}
}