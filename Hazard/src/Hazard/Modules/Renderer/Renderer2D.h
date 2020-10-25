#pragma once
#include "Hazard/Modules/Scene/Scene.h"
#include "Shaders/Shader.h"
#include <glm/glm.hpp>

namespace Hazard {

	class HAZARD_API Renderer2D {

	public:
		void Init();

		void BeginScene();
		void EndScene();
		void Render(Scene* scene);
		void Resize(float w, float h);
	private:
		void RenderEntity(Entity* entity);
		Shader* shader;
		glm::mat4 projection;
	};
}