#pragma once
#include <hzrpch.h>
#include "RenderContext.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"

#include "Hazard/Core/HazardLoop.h"
#include "RenderUtils.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderAPI api) : currentAPI(api), Module::Module("RenderContext")
	{
		SetActive(true);
		window = Window::Create();
		RenderUtils::SetRenderAPI(api);
	}
	RenderContext::~RenderContext()
	{
		Close();
	}

	void RenderContext::Init()
	{
		
		window->SetEventCallback(BIND_EVENT(RenderContext::Process));
	}

	void RenderContext::Update()
	{
		window->OnUpdate(clearColor);
	}

	void RenderContext::Close()
	{
		
	}

	void RenderContext::Process(Event& e)
	{
		Core::HazardLoop::GetCurrent().Process(e);
	}
}