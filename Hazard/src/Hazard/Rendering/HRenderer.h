#pragma once

#include "Hazard/ECS/Component.h"
#include "Hazard/Core/Core.h"
#include "HazardRenderer.h"
#include "RenderEngine.h"
#include "Renderers/WorldRenderer.h"
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
		static void SubmitSprite(TransformComponent& transform, const SpriteRendererComponent& spriteRenderer);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2DAsset> texture);
		//Mesh3D
		static void SubmitMesh(TransformComponent& transform, const MeshComponent& meshComponent);
		static void SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<Pipeline> pipeline, size_t count);
		static void SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline);
		static void SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, size_t count);
		static void SubmitMesh(const RawMesh& rawMesh, Ref<Pipeline> pipeline);

		static void SubmitShadowMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline, size_t count);

		//Pipeline
		static void SubmitPipeline(Ref<Pipeline>& pipeline, size_t count);

		//Light
		static void SubmitSkyLight(const SkyLightComponent& skyLight);
		static void SubmitDirectionalLight(const TransformComponent& transform, DirectionalLightComponent& directionalLight);
		static void SubmitPointLight(const TransformComponent& transform, PointLightComponent& pointLight);

		//Debug
		static void SubmitPerspectiveCameraFrustum(const glm::vec3 position, const glm::quat& orientation, const glm::mat4& transform, float verticalFOV, glm::vec2 clipping, float aspectRatio, const Color& color = Color::White);
		static void SubmitOrthoCameraFrustum(const glm::vec3 position, const glm::quat& orientation, const glm::mat4& transform, float size, glm::vec2 clipping, float aspectRatio, const Color& color = Color::White);
		static void SubmitBoundingBox(const glm::mat4& transform, const BoundingBox& boundingBox, const Color& color = Color::White);

	private:
		inline static RenderEngine* s_Engine;
	};
}