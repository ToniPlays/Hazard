#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Renderer/Shaders/Shader.h"
#include "Hazard/Modules/Renderer/Buffers/VertexArray.h"

namespace Hazard {

	class HAZARD_API SpriteRenderer : public Component {
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void OnRender() override;
		void SetTint(Color _tint);
		Color GetTint() { return tint; };
	private:
		Color tint;
		VertexArray* vertexArray;

	};
}