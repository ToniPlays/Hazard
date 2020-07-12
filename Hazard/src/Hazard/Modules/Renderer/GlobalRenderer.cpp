#pragma once
#include <hzrpch.h>
#include "GlobalRenderer.h"



namespace Hazard {

	GlobalRenderer* GlobalRenderer::Instance;
	float GlobalRenderer::speed = 45.0f;

	GlobalRenderer::GlobalRenderer() : Module("Global renderer")
	{
		if (GlobalRenderer::Instance == nullptr) {
			GlobalRenderer::Instance = this;
		}
	}

	GlobalRenderer::~GlobalRenderer()
	{

	}
	bool GlobalRenderer::OnEnabled()
	{
		api = new RendererAPI();
		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT(GlobalRenderer::OnEvent));

		HZR_ASSERT(window != nullptr, "Window was not created");
		renderer3D = new Renderer3D();
		renderer2D = new Renderer2D();
		grid = new Grid();
		renderTexture = RendererAPI::RenderTexture();

		WindowResizeEvent event(window->GetWidth(), window->GetHeight());
		OnResized(event);

		return true;
	}

	void GlobalRenderer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(GlobalRenderer::OnResized));
		HazardLoop::GetCurrent().OnEvent(e);
	}

	void GlobalRenderer::Render()
	{
		window->OnUpdate();
#if HZR_GAME_ONLY
		renderer2D->Render();
		renderer3D->Render();
		grid->Render();
#endif
	}

	void GlobalRenderer::SceneRender()
	{
		renderTexture->Bind();
		window->GetContext()->ClearFrame();
		//Use 2D and 3D renderers
		renderer2D->Render();
		renderer3D->Render();
		grid->Render();

		renderTexture->Unbind();
		std::stringstream ss;

#if !defined(HZR_GAME_ONLY) && !defined(HZR_RELEASE)
		ss << drawCalls;
		HazardLoop::GetAppInfo().SetValue("DrawCalls", ss.str());
		drawCalls = 0;

#endif // HZR_GAME_ONLY | HZR_RELEASE

	}

	bool GlobalRenderer::OnResized(Event& e)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;

		renderer3D->OnResized(e);
		renderer2D->OnResized(e);

		renderTexture->SetWidth(resizeEvent.GetWidth());
		renderTexture->SetHeight(resizeEvent.GetHeight());

		return true;
	}

	void GlobalRenderer::Draw(VertexArray* mesh, RenderType type)
	{
		if(type == RenderType::None)
			type = GlobalRenderer::Instance->GetAPI().GetType();

		GlobalRenderer* rd = GlobalRenderer::Instance;
		rd->window->GetContext()->Draw(type, mesh);
		rd->drawCalls++;
	}

	Renderer2D* GlobalRenderer::Get2D()
	{
		return renderer2D;
	}

}
