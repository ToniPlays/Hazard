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

		bool IsValid() { return RenderPass; }
	};

	struct WorldRendererSpec
	{
		std::string DebugName;
		Ref<World> TargetWorld = nullptr;

	};

	class WorldRenderer : public RefCount
	{
		friend class RenderEngine;
	public:
		WorldRenderer() = default;
		WorldRenderer(WorldRendererSpec* spec);

		void SetTargetWorld(Ref<World> world);
		void SubmitCamera(const WorldCameraData& camera);

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

		Ref<HazardRenderer::DescriptorSet> GetCameraDescriptor(uint32_t index) const { return m_CameraDescriptors[index]; };

		bool IsValid() const { return m_TargetWorld; }

	private:
		WorldRendererSpec m_Spec;
		Ref<World> m_TargetWorld;
		std::vector<WorldCameraData> m_CameraData;
		std::vector<Ref<HazardRenderer::DescriptorSet>> m_CameraDescriptors;

		Callback<void()> m_RendererExtraCalls;
	};
}