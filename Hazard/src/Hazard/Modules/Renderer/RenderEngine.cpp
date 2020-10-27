#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "Hazard/Modules/Scene/SceneManager.h"



namespace Hazard {

	RenderEngine* RenderEngine::Instance;
	uint32_t RenderEngine::boundShader = 0;

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
		stats = new RendererStats();
		window = std::unique_ptr<Window>(Window::Create());
		HZR_ASSERT(window != nullptr, "Window was not created");
		renderTexture = RendererAPI::Create<RenderTexture>();

		window->SetEventCallback(BIND_EVENT(RenderEngine::OnEvent));
		window->SetWindowIcon("res/icons/hazard_16.ico", "res/icons/hazard_16.ico");

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
		stats->draws = 0;

		renderTexture->Bind();
		window->GetContext()->ClearFrame();

		Scene* scene = ModuleHandler::GetModule<SceneManager>()->GetActiveScene();
		sceneCamera = scene->sceneCamera;

		if (sceneCamera != nullptr) {
			
			sceneCamera->RecalculateViewMatrix(stats->height, stats->width);
			renderer2D.BeginScene(sceneCamera);
			renderer2D.Render(scene);
			renderer2D.EndScene();

		}
		renderTexture->Unbind();
	}

	bool RenderEngine::OnResized(Event& e)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;
		stats->height = resizeEvent.GetWidth();
		stats->width = resizeEvent.GetHeight();
		renderTexture->SetWidth(stats->width);
		renderTexture->SetHeight(stats->height);
		renderer2D.Resize(stats->height, stats->width);

		if(sceneCamera != nullptr)
			sceneCamera->RecalculateViewMatrix(stats->height, stats->width);

		return true;
	}

	void RenderEngine::Draw(VertexArray* array)
	{
		array->BindAll();
		Instance->window->GetContext()->Draw(RenderType::Default, array);
		Instance->stats->draws++;
	}

	std::string RenderEngine::GetError() {
		return Instance->GetWindow().GetContext()->GetError();
	}
}
