#pragma once

#include "HazardRenderer.h"
#include "Hazard/ECS/World.h"

namespace Hazard
{
	using RenderExtraCallback = std::function<void()>;

	struct WorldCameraData 
	{
		glm::mat4 Projection;
		glm::mat4 View;
		glm::vec3 Position;
		float ZNear;
		float ZFar;
		uint32_t Width;
		uint32_t Height;

		Ref<HazardRenderer::RenderPass> RenderPass;
		Ref<HazardRenderer::FrameBuffer> OutputFrameBuffer;

		bool IsValid() { return RenderPass && OutputFrameBuffer; }
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
		std::string DebugName;
		GeometryInclude Geometry = Geometry_All;

	};

	class WorldRenderer : public RefCount 
	{
		friend class RenderEngine;
	public:
		WorldRenderer() = default;
		WorldRenderer(WorldRendererSpec* spec);
		~WorldRenderer() = default;

		void SetTargetWorld(Ref<World> world) 
		{
			m_TargetWorld = world;
		}

		void SubmitCamera(WorldCameraData camera)
		{
			m_CameraData.push_back(camera);
		}
		/// <summary>
		/// Submit to rendering
		/// </summary>
		void Submit();
		void Render();
		void SubmitExtra(const RenderExtraCallback& callback) 
		{
			m_RendererExtraCalls.push_back(callback);
		}
		void OnRenderExtra() 
		{
			for (auto& cb : m_RendererExtraCalls) { cb(); }
		}

		const WorldRendererSpec& GetSpec() const { return m_Spec; }
		Ref<World> GetTargetWorld() const { return m_TargetWorld; }
		std::vector<WorldCameraData> GetCameraData() const { return m_CameraData; }

		bool IsValid() const { return m_TargetWorld; }

	private:
		WorldRendererSpec m_Spec;
		Ref<World> m_TargetWorld;
		std::vector<WorldCameraData> m_CameraData;

		std::vector<RenderExtraCallback> m_RendererExtraCalls;
	};
}