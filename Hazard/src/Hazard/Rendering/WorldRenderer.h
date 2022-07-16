#pragma once

#include "HazardRenderer.h"

namespace Hazard
{
	struct WorldCameraData {
		glm::mat4 ViewProjection;
		glm::mat4 Projection;
		glm::mat4 View;
		glm::vec3 Position;

	};

	enum GeometryInclude
	{
		Geometry_Quad = BIT(0),
		Geometry_Lines = BIT(1),
		Geometry_Mesh = BIT(2),
		Geometry_Text = BIT(3),
		Geometry_Shadow = BIT(4),
		Geometry_All = Geometry_Quad | Geometry_Lines | Geometry_Mesh | Geometry_Shadow
	};

	struct WorldRendererSpec
	{
		uint32_t Width;
		uint32_t Height;
		std::string DebugName;
		HazardRenderer::Camera* Camera;
		GeometryInclude Geometry = Geometry_All;
	};

	class WorldRenderer : public RefCount {
	public:
		WorldRenderer() = default;
		WorldRenderer(WorldRendererSpec* spec);
		~WorldRenderer() = default;

		void Render();

		const WorldRendererSpec& GetSpec() { return m_Spec; }
		const Ref<HazardRenderer::FrameBuffer>& GetOutput() { return m_Output; }
		const Ref<HazardRenderer::RenderPass>& GetRenderPass() { return m_RenderPass; }


	private:
		WorldRendererSpec m_Spec;
		Ref<HazardRenderer::FrameBuffer> m_Output;
		Ref<HazardRenderer::RenderPass> m_RenderPass;
	};
}