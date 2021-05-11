#pragma once
#include <hzrpch.h>
#include "RenderContext.h"
#include "Platform/Rendering/OpenGL/OpenGLContext.h"

#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "RenderContextCommand.h"

namespace Hazard::Rendering {

	RenderContext::RenderContext(RenderAPI api) : m_CurrentAPI(api), Module::Module("RenderContext")
	{
		RenderUtils::SetRenderAPI(api);
	}
	RenderContext::~RenderContext() {}

	void RenderContext::Init()
	{
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT(RenderContext::Process));
		RenderUtils::Init();
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
}