#pragma once

#include "HazardRenderer.h"
#include "Hazard/ECS/World.h"
#include "Callback.h"

namespace Hazard
{
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
		Ref<World> TargetWorld = nullptr;

	};

	class WorldRenderer : public RefCount 
	{
		friend class RenderEngine;
	public:
		WorldRenderer() = default;
		WorldRenderer(WorldRendererSpec* spec);

		void SetTargetWorld(Ref<World> world) 
		{
			HZR_CORE_ASSERT(world, "Target world cannot be nullptr");
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
		void SubmitExtra(const std::function<void()>& callback)
		{
			m_RendererExtraCalls.Add(callback);
		}
		void OnRenderExtra() 
		{
			m_RendererExtraCalls.Invoke();
		}

		const WorldRendererSpec& GetSpec() const { return m_Spec; }
		Ref<World> GetTargetWorld() const { return m_TargetWorld; }
		std::vector<WorldCameraData> GetCameraData() const { return m_CameraData; }

		bool IsValid() const { return m_TargetWorld; }

	private:
		WorldRendererSpec m_Spec;
		Ref<World> m_TargetWorld;
		std::vector<WorldCameraData> m_CameraData;

		Callback<void()> m_RendererExtraCalls;
	};
}