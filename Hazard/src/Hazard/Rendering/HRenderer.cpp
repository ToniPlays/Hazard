
#include <hzrpch.h>
#include "HRenderer.h"
#include "WorldRenderer.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void HRenderer::SubmitWorldRenderer(WorldRenderer* renderer)
	{
		HZR_PROFILE_FUNCTION();
		auto& spec = renderer->GetSpec();

		RenderingCamera camera = {};
		camera.DebugName = spec.DebugName;
		camera.GeometryFlags = spec.Geometry;
		camera.ViewProjection = spec.Camera->GetViewProjection();
		camera.Projection = spec.Camera->GetProjection();
		camera.View = spec.Camera->GetView();
		camera.Position = spec.Camera->GetPosition();
		camera.RenderPass = renderer->GetRenderPass();

		s_Engine->GetDrawList().RenderingCameras.push_back(camera);
	}

	void HRenderer::SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer)
	{
		glm::mat4& tMatrix = transform.GetTransformMat4();
		const Color& t = spriteRenderer.Color;
		glm::vec4 color = { t.r, t.g, t.b, t.a };

		SubmitQuad(tMatrix, color, spriteRenderer.Texture);
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		SubmitQuad(transform, color, nullptr);	
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture)
	{
		s_Engine->GetQuadRenderer().SubmitQuad(transform, color, texture);
	}
	void HRenderer::SubmitMesh(const TransformComponent& transform, const MeshComponent& meshComponent)
	{
		Ref<Mesh> mesh = meshComponent.m_MeshHandle;
		if (!mesh) return;
		SubmitMesh(transform.GetTransformMat4(), mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetPipeline());
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline)
	{
		s_Engine->GetDrawList().Meshes[pipeline.Raw()].push_back({ transform, vertexBuffer, indexBuffer, indexBuffer->GetCount() });
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline, uint32_t count)
	{
		s_Engine->GetDrawList().Meshes[pipeline.Raw()].push_back({ transform, vertexBuffer, indexBuffer, count });
	}
	void HRenderer::SubmitPipeline(Ref<Pipeline>& pipeline, uint32_t count)
	{
		s_Engine->GetDrawList().Pipelines[pipeline.Raw()].push_back({ count });
	}
	void HRenderer::SubmitSkyLight(const SkyLightComponent& skyLight)
	{
		s_Engine->GetDrawList().Environment.push_back({ 1.0f });
	}
	void HRenderer::SubmitDirectionalLight(const TransformComponent& transform, DirectionalLightComponent& directionalLight)
	{
		s_Engine->GetDrawList().LightSource.push_back({ transform.GetTransformNoScale(), directionalLight.LightColor, directionalLight.Intensity });
	}
	void HRenderer::SubmitPointLight(const TransformComponent& transform, PointLightComponent& pointLight)
	{
		s_Engine->GetDrawList().LightSource.push_back({ transform.GetTransformNoScale(), pointLight.LightColor, pointLight.Intensity });
	}
}