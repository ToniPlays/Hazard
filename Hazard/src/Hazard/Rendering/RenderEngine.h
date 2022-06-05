#pragma once

#include "Hazard/Module.h"
#include "HazardRendererCore.h"

namespace Hazard 
{
	class RenderEngine : public Module 
	{
	public:
		RenderEngine() = default;
		RenderEngine(HazardRenderer::HazardRendererCreateInfo* createInfo);
		~RenderEngine() = default;

		void Render() override;
		void PostRender() override;

		HazardRenderer::Window& GetWindow() { return *m_Window; }

	private:
		HazardRenderer::Window* m_Window;
	};
}