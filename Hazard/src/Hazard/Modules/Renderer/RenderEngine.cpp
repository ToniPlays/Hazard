#pragma once
#include <hzrpch.h>
#include "RenderEngine.h"
#include "Hazard/Modules/Scene/SceneManager.h"



namespace Hazard {

	enum class DebugLogLevel
	{
		None = 0, HighAssert = 1, High = 2, Medium = 3, Low = 4, Notification = 5
	};
	DebugLogLevel glLogLevel = DebugLogLevel::High;

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

		window = std::unique_ptr<Window>(Window::Create());
		HZR_ASSERT(window != nullptr, "Window was not created");
		renderTexture = RendererAPI::Create<RenderTexture>();


		window->SetEventCallback(BIND_EVENT(RenderEngine::OnEvent));
		window->SetWindowIcon("res/icons/logo.png", "res/icons/logo.png");
		renderer2D.Init();

		WindowResizeEvent event(window->GetWidth(), window->GetHeight());
		OnResized(event);

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
		//if (window->IsMinimized()) return;
		if (sceneCamera != nullptr) window->OnUpdate(sceneCamera->clearColor);
		else window->OnUpdate();

		SceneRender();
		PROFILE_FN_END();
	}

	void RenderEngine::SceneRender()
	{
		PROFILE_FN();
		stats.draws = 0;
		stats.quads = 0;
		stats.indices = 0;
		stats.batches = 0;

		renderTexture->Bind();

		Scene* scene = ModuleHandler::GetModule<SceneManager>()->GetActiveScene();
		this->sceneCamera = scene->sceneCamera;

		if (sceneCamera != nullptr) 
		{
			sceneCamera->RecalculateViewMatrix();
			window->GetContext()->ClearFrame(sceneCamera->clearColor);
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
		stats.width = width;
		stats.height = height;
		renderTexture->Resize(width, height);
		if (sceneCamera != nullptr) 
			sceneCamera->RecalculateViewMatrix();
	}

	void RenderEngine::SetSceneCamera(CameraComponent* camera)
	{
		if (sceneCamera == camera) return;
		camera->RecalculateViewMatrix();
		sceneCamera = camera;
	}

	void RenderEngine::CheckError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			if ((int)glLogLevel > 0)
			{
				HZR_CORE_ERROR("[OpenGL Debug HIGH]" + std::string(message));
				if (glLogLevel == DebugLogLevel::HighAssert)
					HZR_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
			}
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			if ((int)glLogLevel > 2)
				HZR_CORE_INFO("[OpenGL Debug MEDIUM] " + std::string(message));
			break;
		case GL_DEBUG_SEVERITY_LOW:
			if ((int)glLogLevel > 3)
				HZR_CORE_INFO("[OpenGL Debug LOW] " + std::string(message));
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			if ((int)glLogLevel > 4)
				HZR_CORE_TRACE("[OpenGL Debug NOTIFICATION] " + std::string(message));
			break;
		}
	}

	void RenderEngine::Draw(VertexArray* array, uint32_t indices)
	{
		Instance->window->GetContext()->DrawIndexed(array, indices);
		Instance->stats.draws++;
		Instance->stats.indices += indices;
	}
}
