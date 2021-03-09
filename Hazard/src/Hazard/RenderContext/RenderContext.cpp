#pragma once
#include <hzrpch.h>
#include "RenderContext.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"

#include "Hazard/Core/HazardLoop.h"
#include "RenderUtils.h"
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderAPI api) : currentAPI(api), Module::Module("RenderContext")
	{
		RenderUtils::SetRenderAPI(api);
	}
	RenderContext::~RenderContext()
	{

	}

	void RenderContext::Init()
	{
		SetActive(true);
		window = Window::Create();
		window->SetEventCallback(BIND_EVENT(RenderContext::Process));
		RenderContextCommand::Init();
	}

	void RenderContext::Update()
	{
		window->OnUpdate(clearColor);
	}

	void RenderContext::Close()
	{
		delete window;
	}

	void RenderContext::Process(Event& e)
	{
		Core::HazardLoop::GetCurrent().Process(e);
	}
}