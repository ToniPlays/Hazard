#pragma once
#include "Hazard/Modules/Scene/Scene.h"
#include "Shaders/Shader.h"
#include <glm/glm.hpp>

namespace Hazard {

	class HAZARD_API Renderer2D {

	public:

		Renderer2D() {}
		void Init();

		void BeginScene(CameraComponent* sceneCamera);
		void EndScene();
		void Render(Scene* scene);
		void Resize(float w, float h);
	private:
		CameraComponent* sceneCamera = nullptr;
		void RenderEntity(Entity* entity);
		Shader* shader;
		glm::mat4 projection;
	};
}