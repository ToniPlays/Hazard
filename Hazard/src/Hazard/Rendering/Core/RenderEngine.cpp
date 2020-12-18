#pragma once

#include <hzrpch.h>
#include "RenderEngine.h"
#include "RendererHandler.h"

#include "Hazard/ECS/SceneManager.h"
#include "Hazard/ECS/Components.h"
#include "Hazard/ECS/Entity.h"

#include "glad/glad.h"

namespace Hazard {

	VertexArray* vArray;
	Shader* shader;


	RenderEngine::RenderEngine() : Module("RenderEngine") 
	{
		
	}

	RenderEngine::~RenderEngine() 
	{
		
	}

	bool RenderEngine::OnEnabled() {

		RendererHandler::Init(Renderer::OpenGL);

		float data[] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		window = Window::Create();

		window->SetEventCallback(BIND_EVENT(RenderEngine::ProcessEvent));
		window->GetContext()->SetErrorListener(BIND_EVENT(RenderEngine::OnError));


		renderTexture = RendererHandler::Create<RenderTexture>();
		shader = RendererHandler::Create<Shader>("res/shaders/default.glsl");
		
		
		vArray = RendererHandler::Create<VertexArray>();
		VertexBuffer* buffer = RendererHandler::Create<VertexBuffer>(1000);

		buffer->SetData(data, sizeof(data));

		buffer->SetLayout(
			{ { ShaderDataType::Float3, "position" } }
			);

		vArray->AddBuffer(buffer);

		IndexBuffer* indexBuffer = RendererHandler::Create<IndexBuffer>();

		indexBuffer->SetData(indices, 6);
		vArray->SetIndexBuffer(indexBuffer);

		return true;
	}
	bool RenderEngine::OnDisabled() {
		return true;
	}

	void RenderEngine::ProcessEvent(Event& e) {
		HazardLoop::GetCurrent().OnEvent(e);
	}

	void RenderEngine::SetTitle(const char* title)
	{
		std::stringstream ss;
		ss << title << " | Hazard " << HZR_BUILD_VERSION;
		window->SetWindowTitle(ss.str().c_str());
	}

	void RenderEngine::Render() 
	{
		ECS::Scene* scene = &ModuleHandler::GetModule<ECS::SceneManager>()->GetActiveScene();
		renderTexture->Bind();
		window->OnUpdate(Color::FromHex("#505050"));

		shader->Bind();

		/*scene->GetRegistry().each([&](auto entityID) {
			ECS::Entity entity{ entityID, scene };
			
		});*/

		window->GetContext()->DrawArray(vArray);
		renderTexture->Unbind();
		shader->Unbind();
	}

	void RenderEngine::OnViewResized(float w, float h) {
		stats.width = w;
		stats.height = h;
		renderTexture->Resize(w, h);
	}
	void RenderEngine::OnError(ErrorData& data)
	{
		HZR_CORE_INFO("Engine goes brrr");
	}
}