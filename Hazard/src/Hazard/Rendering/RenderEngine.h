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

	private:
		HazardRenderer::Window* m_Window;
	};
}