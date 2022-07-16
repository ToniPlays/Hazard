
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

		s_Engine->GetQuadRenderer().SubmitQuad(tMatrix, color);
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
}