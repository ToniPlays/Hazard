#pragma once

#include "Hazard/Core/Core.h"
#include "Camera.h"
#include "Pipeline/FrameBuffer.h"
#include "Pipeline/RenderPass.h"
#include "Queue/RenderCommandBuffer.h"
#include "Queue/RenderCommandQueue.h"

#include <glm/glm.hpp>

namespace Hazard::Rendering 
{
	enum WorldRenderFlags_
	{
		Enabled =			BIT(0),
		Geometry =			BIT(1),
		Quads =				BIT(2),
		Lines =				BIT(3),
		Points =			BIT(4),
		DirectionalLight =	BIT(5),
		PointLight =		BIT(6)
	};

	struct WorldRendererSettings
	{
		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		Camera* Camera = nullptr;
		glm::vec2 ViewportSize = { 1, 1 };
		bool SwapchainTarget = false;
		bool IsDebugRenderer = false;
		float LineWidth = 1.0f;
		int Flags;
	};

	class WorldRenderer : public RefCount
	{
		friend class RenderEngine;
	public:
		WorldRenderer();
		WorldRenderer(WorldRendererSettings* settings);
		~WorldRenderer();

		void Begin(Ref<RenderCommandBuffer> cmdBuffer, RenderCommandQueue* queue);
		void End(Ref<RenderCommandBuffer> cmdBuffer);
		
		void SetViewport(uint32_t width, uint32_t height);
		WorldRendererSettings& GetSettings() { return m_Settings; }
		bool IsValid() { return m_Settings.Camera != nullptr; }
		bool Prepare();

		Ref<Image2D> GetFinalPassImage() { return m_FrameBuffer->GetImage(); }
		Ref<RenderPass> GetRenderPass() { return m_RenderPass; }

		static Ref<WorldRenderer> Create(WorldRendererSettings* settings) { return new WorldRenderer(settings); }

	private:
		void Invalidate();
		void CreateResources();

	private:

		Ref<FrameBuffer> m_FrameBuffer;
		Ref<RenderPass> m_RenderPass;
		WorldRendererSettings m_Settings;
		bool m_Resize = false;

		static inline std::vector<WorldRenderer*> s_Renderers;
	};
}