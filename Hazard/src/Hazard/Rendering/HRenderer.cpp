
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
		camera.ViewProjection = spec.Camera.GetViewProjection();
		camera.Position = spec.Camera.GetPosition();
		camera.RenderPass = renderer->GetRenderPass();

		s_Engine->GetDrawList().RenderingCameras.push_back(camera);
	}

	void HRenderer::SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer)
	{

	}
}