#include "hzrpch.h"
#include "RenderEngine.h"
#include "HazardRenderer.h"

namespace Hazard
{
	using namespace HazardRenderer;

	RenderEngine::RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo) : Module("RenderEngine")
	{
		m_Window = Window::Create(createInfo);
		m_Window->Show();
	}
    void RenderEngine::Update()
    {
        m_Window->BeginFrame();
    }
	void RenderEngine::Render()
	{
		
	}
	void RenderEngine::PostRender()
	{
		m_Window->Present();
		Renderer::WaitAndRender();
	}
}
