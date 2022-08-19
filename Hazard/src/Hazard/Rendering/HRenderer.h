#pragma once

#include "Hazard/ECS/Component.h"
#include "HazardRenderer.h"
#include "RenderEngine.h"
#include "WorldRenderer.h"
#include "Color.h"

namespace Hazard
{
	using namespace HazardRenderer;
	class HRenderer
	{
		friend class RenderEngine;
		friend class QuadRenderer;
		friend class LineRenderer;
	public:
		static void SubmitWorldRenderer(WorldRenderer* renderer);

		//Mesh 2D
		static void SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& textur6e);
		//Mesh3D
		static void SubmitMesh(const TransformComponent& transform, const MeshComponent& meshComponent);
		static void SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<Pipeline>& pipeline, uint32_t count);
		static void SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline);
		static void SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline, uint32_t count);

		//Pipeline
		static void SubmitPipeline(Ref<Pipeline>& pipeline, uint32_t count);
		//Light
		static void SubmitSkyLight(const SkyLightComponent& skyLight);
		static void SubmitDirectionalLight(const TransformComponent& transform, DirectionalLightComponent& directionalLight);
		static void SubmitPointLight(const TransformComponent& transform, PointLightComponent& pointLight);

		//Debug
		static void DrawCameraFrustum(const glm::vec3 position, const glm::quat& orientation, const glm::mat4& transform, float verticalFOV, float zNear, float zFar, float aspectRatio, const Color& color = Color::White);

	private:
		inline static RenderEngine* s_Engine;
	};
}