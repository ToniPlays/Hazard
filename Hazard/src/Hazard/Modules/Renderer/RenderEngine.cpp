#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "Hazard/Modules/Scene/SceneManager.h"



namespace Hazard {

	RenderEngine* RenderEngine::Instance;
	Shader* RenderEngine::currentShader = nullptr;

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
		PROFILE_FN();
		if (sceneCamera != nullptr) window->OnUpdate(sceneCamera->clearColor);
		else window->OnUpdate();

		SceneRender();
		PROFILE_FN_END();
	}

	void RenderEngine::SceneRender()
	{
		PROFILE_FN();
		stats->draws = 0;
		stats->quads = 0;
		stats->indices = 0;

		renderTexture->Bind();

		Scene* scene = ModuleHandler::GetModule<SceneManager>()->GetActiveScene();
		this->sceneCamera = scene->sceneCamera;

		if (sceneCamera != nullptr) 
		{
			window->GetContext()->ClearFrame(sceneCamera->clearColor);
			sceneCamera->RecalculateViewMatrix();
			renderer2D.Render(scene);

		}
		else window->GetContext()->ClearFrame();
		renderTexture->Unbind();

		PROFILE_FN_END();
	}

	bool RenderEngine::OnResized(Event& e)
	{
		WindowResizeEvent& resizeEvent = (WindowResizeEvent&)e;
		
		renderer2D.Resize(resizeEvent.GetWidth(), resizeEvent.GetHeight());

		if(sceneCamera != nullptr)
			sceneCamera->RecalculateViewMatrix();
		return true;
	}
	void RenderEngine::OnViewResized(uint32_t width, uint32_t height) {
		stats->width = width;
		stats->height = height;
		renderTexture->Resize(width, height);
	}

	void RenderEngine::Draw(VertexArray* array, uint32_t indices)
	{
		array->Bind();
		Instance->window->GetContext()->Draw(array, indices);
		Instance->stats->draws++;
		Instance->stats->indices += indices;
	}

	std::string RenderEngine::GetError() {
		return Instance->GetWindow().GetContext()->GetError();
	}
}
