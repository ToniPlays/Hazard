#pragma once
#include <hzrpch.h>
#include "Renderer2D.h"
#include "Shaders/Shader.h"
#include "RenderEngine.h"
#include "Buffers/VertexArray.h"

namespace Hazard {

	void Renderer2D::Init()
	{
		shader = RendererAPI::Create<Shader>("res/shaders/canvasShader.glsl");
	}

	void Renderer2D::BeginScene(CameraComponent* sceneCamera) {
		this->sceneCamera = sceneCamera;

		shader->Bind();
		RenderEngine::boundShader = shader->GetProgram();
		shader->SetUniform("viewProjection", sceneCamera->GetViewProjection());
	}
	void Renderer2D::EndScene() {
		
	}
	void Renderer2D::Render(Scene* scene)
	{
		for (Entity* entity : scene->GetChildEntities()) {
			RenderEntity(entity);
		}
	}
	void Renderer2D::Resize(float w, float h)
	{
		float scale = 5;
		float aspectX = w / h * scale;
		std::cout << aspectX << "x" << scale << std::endl;
		projection = glm::ortho(-aspectX, aspectX, -scale, scale, -1.0f, 1.0f);
		//projection = glm::perspective((float)Math::ToRadians(60.0f), w / (float)h, 0.1f, 1000.0f);
	}
	void Renderer2D::RenderEntity(Entity* entity) {
		if (!entity->HasComponent<Transform>()) return;
		shader->SetUniform("transform", Transform::AsMat4(*entity->GetComponent<Transform>()));

		for (Component* component : entity->GetComponents()) {
			component->OnRender();
		}

		for (Entity* entity : entity->GetChildEntities()) {
			RenderEntity(entity);
		}
	}
}
