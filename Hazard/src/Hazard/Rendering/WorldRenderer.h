#pragma once

#include "Hazard/Core/Core.h"
#include "Camera.h"
#include "Pipeline/FrameBuffer.h"
#include <glm/glm.hpp>

namespace Hazard::Rendering 
{
	struct WorldRendererSettings
	{
		glm::vec4 ClearColor;
		Camera* Camera;
		glm::vec2 ViewportSize;
		Ref<FrameBuffer> Target = nullptr;
		bool SwapchainTarget = false;
		bool IsDebugRenderer = false;
	};

	class WorldRenderer : public RefCount
	{
	public:
		WorldRenderer() = default;
		WorldRenderer(WorldRendererSettings* settings) : m_Settings(std::move(*settings)) {};
		~WorldRenderer() = default;
		
		void SetViewport(uint32_t width, uint32_t height);
		void Invalidate();

		void Begin();
		void End();

		WorldRendererSettings GetSettings() { return m_Settings; }

		static Ref<WorldRenderer> Create() { return Ref<WorldRenderer>::Create(); };
		static Ref<WorldRenderer> Create(WorldRendererSettings* settings) { return Ref<WorldRenderer>::Create(settings); };

	private:
		WorldRendererSettings m_Settings;
	};
}