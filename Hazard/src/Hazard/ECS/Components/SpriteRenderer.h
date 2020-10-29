#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/ECS/ECS.h"
#include "Hazard/Modules/Renderer/Shaders/Shader.h"
#include "Hazard/Modules/Renderer/Buffers/VertexArray.h"
#include <yaml-cpp/yaml.h>

namespace Hazard {

	class HAZARD_API SpriteRenderer : public Component {
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void OnAttach() override;
		void OnRender() override;
		void SetTint(Color _tint);
		Color GetTint() { return tint; };

		void SerializeComponent(YAML::Emitter& out) override;
		void DeserializeComponent(YAML::Node in) override;

		int textureIndex = 0;

	private:
		Color tint;
		VertexArray* vertexArray;

	};
}