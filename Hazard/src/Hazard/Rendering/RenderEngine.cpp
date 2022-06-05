#include "hzrpch.h"
#include "RenderEngine.h"

namespace Hazard 
{
	using namespace HazardRenderer;

	RenderEngine::RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		m_Window = Window::Create(createInfo);
		m_Window->Show();
	}
	void RenderEngine::Render()
	{
		m_Window->BeginFrame();

		
	}
	void RenderEngine::PostRender() 
	{
		m_Window->Present();
	}
}
