
#include <hzrpch.h>
#include "HRenderer.h"
#include "WorldRenderer.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void HRenderer::SubmitWorldRenderer(WorldRenderer* renderer)
	{
		auto& spec = renderer->GetSpec();

		RenderingCamera camera = {};
		camera.DebugName = spec.DebugName;
		camera.GeometryFlags = spec.Geometry;
		camera.ViewProjection = spec.Camera->GetViewProjection();
		camera.Position = spec.Camera->GetPosition();
		camera.RenderPass = renderer->GetRenderPass();

		s_Engine->GetDrawList().RenderingCameras.push_back(camera);
	}

	void HRenderer::SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer)
	{
		glm::mat4& tMatrix = transform.GetTransformMat4();
		const Color& t = spriteRenderer.Tint;
		glm::vec4 color = { t.r, t.g, t.b, t.a };

		SubmitQuad(tMatrix, color);
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		s_Engine->GetQuadRenderer().SubmitQuad(transform, color);
	}
	void HRenderer::SubmitMesh(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline)
	{
		auto& cmdBuffer = s_Engine->GetWindow().GetSwapchain()->GetSwapchainBuffer();
		vertexBuffer->Bind(cmdBuffer);
		indexBuffer->Bind(cmdBuffer);
		pipeline->Bind(cmdBuffer);
		pipeline->Draw(cmdBuffer, indexBuffer->GetCount());
	}
	void HRenderer::SubmitMesh(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline, uint32_t count)
	{
		auto& cmdBuffer = s_Engine->GetWindow().GetSwapchain()->GetSwapchainBuffer();
		vertexBuffer->Bind(cmdBuffer);
		indexBuffer->Bind(cmdBuffer);
		pipeline->Bind(cmdBuffer);
		pipeline->Draw(cmdBuffer, count);
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