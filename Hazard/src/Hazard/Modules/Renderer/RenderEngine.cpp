#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "Hazard/Modules/Scene/SceneManager.h"



namespace Hazard {

	RenderEngine* RenderEngine::Instance;

	RenderEngine::RenderEngine() : Module("RenderEngine")
	{
		if (RenderEngine::Instance == nullptr)
			RenderEngine::Instance = this;
	}

	RenderEngine::~RenderEngine()
	{

	}

	bool RenderEngine::OnEnabled()
	{
		api = new RendererAPI(RenderAPI::OpenGL);

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT(RenderEngine::OnEvent));

		window->SetWindowIcon("res/icons/hazard_16.ico", "res/icons/hazard_16.ico");

		HZR_ASSERT(window != nullptr, "Window was not created");
		renderTexture = RendererAPI::RenderTexture();

		WindowResizeEvent event(window->GetWidth(), window->GetHeight());
		OnResized(event);
		renderer2D.Init();
		
		return true;
	}

	void RenderEngine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(RenderEngine::OnResized));
		HazardLoop::GetCurrent().OnEvent(e);
	}

	void RenderEngine::Render()
	{
		window->OnUpdate();
#ifdef HZR_GAME_ONLY
		Scene* scene = ModuleHandler::GetModule<SceneManager>()->GetActiveScene();
		renderer2D.BeginScene();
		renderer2D.Render(scene);
		renderer2D.EndScene();
#endif
	}

	void RenderEngine::SceneRender()
	{
		Scene* scene = ModuleHandler::GetModule<SceneManager>()->GetActiveScene();
		renderTexture->Bind();
		window->GetContext()->ClearFrame();

		renderer2D.BeginScene();
		renderer2D.Render(scene);
		renderer2D.EndScene();

		//Use 2D and 3D renderers
		renderTexture->Unbind();
	}

	bool RenderEngine::OnResized(Event& e)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;
		renderTexture->SetWidth(resizeEvent.GetWidth());
		renderTexture->SetHeight(resizeEvent.GetHeight());
		renderer2D.Resize(resizeEvent.GetWidth(), resizeEvent.GetHeight());

		return true;
	}

	void RenderEngine::Draw(VertexArray* array)
	{
		array->BindAll();
		Instance->window->GetContext()->Draw(RenderType::Default, array);
	}

	std::string RenderEngine::GetError() {
		return Instance->GetWindow().GetContext()->GetError();
	}
}
